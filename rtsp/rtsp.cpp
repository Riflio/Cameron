#include "rtsp.h"
#include <QRegularExpression>
#include <QAuthenticator>
#include "estd/estd.h"
#include "rtsp/rtsp_channel.h"

namespace NS_RSTP {

RTSP::RTSP(QObject *parent) : QObject(parent), _reqID(0)
{
  _sckConnect =new QTcpSocket(this);
  _sdp =new SDP(this);
  connect(_sckConnect, &QTcpSocket::readyRead, this, &RTSP::onSckConnectReadyRead);
  connect(_sckConnect, &QTcpSocket::connected, this, &RTSP::onSckConnectConnected);
  connect(_sckConnect, &QTcpSocket::disconnected, this, &RTSP::onSckConnectDisconnected);
  connect(_sckConnect, &QAbstractSocket::errorOccurred, this, &RTSP::onSckConnectError);
}

/**
* @brief Начинаем подключение к камере
* @param url
*/
void RTSP::cameraConnect(QString url)
{
  _gateway =url;
  qInfo()<<"RTSP camera connect url"<<_gateway;
  _authenticator.user =_gateway.userName();
  _authenticator.password =_gateway.password();
  _sckConnect->connectToHost(_gateway.host(), _gateway.port());
}

/**
* @brief Отключаемся от камеры
*/
void RTSP::cameraDisconnect()
{
  qDeleteAll(_channels);
  _channels.clear();
  _sckConnect->close();
}

/**
* @brief Подконнектится получилось, начинаем общение
*/
void RTSP::onSckConnectConnected()
{
  qInfo()<<"Camera socket connected";
  //-- Первым дело запрашиваем, какие комманды поддерживаются
  options();
}

/**
* @brief Запросим камеру о доступных командах
*/
void RTSP::options()
{
  qInfo()<<"RTSP options";
  THeaders headers;
  int r =send(OPTIONS, headers, -1);
  if ( r!=0 ) { emit errored(); return; }
}

/**
* @brief Запросим камеру о доступных каналах и их свойствах
*/
void RTSP::describe()
{
  qInfo()<<"RTSP describe";
  THeaders headers;
  int r =send(DESCRIBE, headers, -1);
  if ( r!=0 ) { emit errored(); return; }
}

/**
* @brief Пришёл ответ от камеры на наш запрос
*
*/
void RTSP::onSckConnectReadyRead()
{
  const QByteArray responseData =_sckConnect->readAll();

  qInfo()<<"RTSP answer"<<responseData;
  if ( responseData.left(RTSPVERSION.length())!=RTSPVERSION ) { qWarning()<<"Unknown protocol!"; return; }

  THeaders respHeaders; //-- Заголовки
  QByteArray respBody; //-- Тело запроса

  //-- Найдём где у нас заголовки кончаются
  uint32_t headersEndIndex =responseData.indexOf("\r\n\r\n")+4;

  //-- Разбираем заголовки
  QByteArray headerName ="status", curBuf; //-- После версии сразу идёт статус, для него нет значения заголовка, так что делаем явно
  for (uint32_t i=RTSPVERSION.length()+1; i<headersEndIndex; ++i) { //-- Пропускаем версию и начинаем разбор
    if ( responseData[i]=='\r' && responseData[i+1]=='\n' ) { if ( !curBuf.isEmpty() ) { respHeaders.append({headerName.toLower(), curBuf}); } headerName.clear(); curBuf.clear(); i++; continue; }
    if ( responseData[i]==':' ) { headerName =curBuf; curBuf.clear(); ++i; continue; }
    curBuf.push_back(responseData[i]);
  }

  //-- Оставшееся после заголовков до конца это тело
  respBody =responseData.mid(headersEndIndex, -1);

  //-- Проверим наличие обязательных полей и значения
  auto cseqIt =std::find_if(respHeaders.begin(), respHeaders.end(), CompareFirst<QString, QString>("cseq"));
  auto statusIt =std::find_if(respHeaders.begin(), respHeaders.end(), CompareFirst<QString, QString>("status"));

  if ( cseqIt==respHeaders.end() ) { qWarning()<<"Has no Cseq param!"; emit errored(); return; }

  //-- Нужно узнать на какой запрос этот ответ
  ReqHistory historyReq;
  uint32_t cseq =cseqIt->second.toInt();
  if ( !reqHistories.contains(cseq) ) { qWarning()<<"There was no request Cseq"<<cseq; emit errored(); return; }
  historyReq =reqHistories.take(cseq);

  if ( statusIt->second!="200 OK" ) {
    if ( statusIt->second=="401 Unauthorized" ) {
     _authenticator.parseHttpResponse(respHeaders, false, _gateway.host());
     if ( _authenticator.phase==QAuthenticatorPrivate::Invalid ) { qWarning()<<"Unable parse WWW-Authenticate"; emit errored(); return; }
     options(); //-- В любом случае заново запрашиваем опции что бы начать уже с авторизацией
     return;
    }
    //-- Не знаем больше что делать при этом статусе, вываливаемся в ошибку
    emit errored();
    return;
  }

  //-- Реагируем на команды
  switch (historyReq.method) {
    case OPTIONS: {
      describe();
      break;
    }
    case DESCRIBE: { //-- Парсим каналы
      //-- Распарсим sdp данные
      if ( !_sdp->parse(respBody) ) { qWarning()<<"Unable parse SDP!"; return; }
      qDebug()<<"SDP channels count"<<_sdp->medias.length();
      for (int i=0; i<_sdp->medias.length(); ++i) {
        RTSP_Channel *channel =new RTSP_Channel(this);
        //-- Отлавливаем события
        connect(channel, &RTSP_Channel::errored, this, &RTSP::errored);
        channel->_sdpMedia =_sdp->medias.at(i);
        _channels.append(channel);
      }
      emit connected();
      break;
    }
    case NONE: {
      break;
    }
    default: {
      //-- Конкретные действия отдаём на обработку самому каналу
      if ( historyReq.channel<0 )  { emit errored(); return; }
      int r =_channels.at(historyReq.channel)->onRTSPReply(historyReq.method, respHeaders);
      if ( r!=0 ) { emit errored(); return; }
      break;
    }
  }
}

/**
* @brief Подготавливаем и отправляем запрос к камере
* @param method
* @param params
* @param channel
* @return
*/
int RTSP::send(METHODS method, const THeaders &headers, uint16_t channel)
{
  QString sMethod;
  switch (method) {
    case OPTIONS: { sMethod ="OPTIONS"; break; }
    case DESCRIBE: { sMethod ="DESCRIBE"; break; }
    case SETUP: { sMethod ="SETUP"; break; }
    case PLAY: { sMethod ="PLAY"; break; }
    case GET_PARAMETER: { sMethod ="GET_PARAMETER"; break; }
    case TEARDOWN: { sMethod ="TEARDOWN"; break; }
    case ALIVE: { sMethod ="GET_PARAMETER"; break; } //-- Не хотят некоторые камеры воспринимать ALIVE, а вот GET_PARAMETER проходит
    default: { qWarning()<<"Unsupported method!"; return -1; }
  }

  _reqID++;

  //-- Запишем базовую необходимую инфу
  _sckConnect->write(QString("%1 rtsp://%2:%3%4 RTSP/1.0\r\n").arg(sMethod).arg(_gateway.host()).arg(_gateway.port()).arg((!_gateway.query().isEmpty())? _gateway.query().prepend("?") :"").toUtf8());
  _sckConnect->write(QString("CSeq: %1\r\n").arg(_reqID).toUtf8());
  _sckConnect->write(QString("User-Agent: %1\r\n").arg("QRTSP").toUtf8());
  if ( _authenticator.method!=QAuthenticatorPrivate::Method::None ) {
    _sckConnect->write(QString("Authorization: %1\r\n").arg(_authenticator.calculateResponse(sMethod.toUtf8(), _gateway.toString().toUtf8(), _gateway.host())).toUtf8());
  }

  //-- Пробегаемся по всем параметрам и добавляем к запросу
  foreach(const THeader &header, headers) {
    _sckConnect->write(QString("%1: %2\r\n").arg(header.first).arg(header.second).toUtf8());
  }

  //-- Заканчиваем пустой строкой
  _sckConnect->write("\r\n");

  //-- Запишем в историю
  reqHistories.insert(_reqID, ReqHistory(method, channel));

  //-- Всё ок
  return 0;
}

/**
* @brief Отдаём количество каналов
* @return
*/
int RTSP::channelsCount()
{
  return _channels.count();
}

/**
* @brief Отдаём конкретный канал
* @param id
* @return
*/
RTSP_Channel *RTSP::getChannel(int id)
{
  return (id<channelsCount()) ? _channels[id] : nullptr;
}

void RTSP::onSckConnectDisconnected()
{
  qInfo()<<"Camera socket disconnected";
  emit disconnected();
}

void RTSP::onSckConnectError(QAbstractSocket::SocketError)
{
  qWarning()<<"Camera socket error: "<< _sckConnect->errorString();
  emit errored();
}


RTSP::~RTSP()
{
  qDebug()<<"";
  cameraDisconnect();
  _sckConnect->deleteLater();
}

}
