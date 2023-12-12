#include "server_client.h"
#include "server.h"
#include <QRegularExpression>
#include <QRandomGenerator>
#include <QDebug>

QMap<quint32, Server_Client::TSessionInfo> Server_Client::_sessions;

Server_Client::Server_Client(QObject *parent, QTcpSocket * socket, Server * server , int clientID) : Server_Client_Info(parent), _socket(socket), _server(server), _id(clientID)
{
  connect(_socket, &QTcpSocket::readyRead, this, &Server_Client::request);

  //-- Клиент должен высылать подтверждение каждые N секунд, если с последнего запроса клиента о подтверждении прошло N времени, значит клиент больше недоступен
  _aliveTimeOverTimer =new QTimer(this);
  _aliveTimeOverTimer->setInterval(120000);
  _aliveTimeOverTimer->start();
  connect(_aliveTimeOverTimer, &QTimer::timeout, this, &Server_Client::aliveTimeOver);

  qDebug()<<"New client ID:"<<_id;
}

int Server_Client::clientID()
{
  return _id;
}

/**
* @brief Возникли ошибки с камерой
*/
void Server_Client::onCameraErrored()
{
  deleteLater(); //TODO: Чё делать то??
}

/**
* @brief Есть новый запрос
*/
void Server_Client::request()
{
  QString data =_socket->readAll();
  _requestData +=data;

  qInfo()<<"\n\n\n"<<"New client request"<<data;

  int commandEndIndex =_requestData.indexOf("\r\n\r\n");

  if (commandEndIndex==-1) { //-- Команда полностью завершена только после пары \r\n, до этого момента накапливаем
    return;
  }

  //-- Разбираем по строкам
  QStringList dataList =_requestData.split("\r\n");

  _requestData ="";

  //-- Берём первую строку, узнаём команду, исходный юрл и версию ртсп
  QString initLine =dataList.takeAt(0);

  //-- Парсим
  QRegularExpression rxInit("([A-Z_]*)\\s([^\\s]*)\\s(.*)");

  QRegularExpressionMatch rxInitMath =rxInit.match(initLine);
  if ( !rxInitMath.hasMatch() ) { //-- пришла какая-то фигня, пропускаем
    qWarning()<<"Unable understand request!";
    answer(400);
    return;
  }

  if ( rxInitMath.captured(3)!="RTSP/1.0" ) {
    qWarning()<<"Wrong RTSP version O_o, check request!";
    answer(400);
    return;
  }

  QString command =rxInitMath.captured(1);
  QUrl reqSource =QUrl(rxInitMath.captured(2));

  if ( !reqSource.isValid() ) {
    qWarning()<<"Request source URL invalid!";
    answer(404);
    return;
  }

  //-- Парсим параметры строки запроса
  QRegularExpressionMatch rxTrackIDMatch =QRegularExpression("/track/([\\d]*)").match(reqSource.path());
  int trackID =(rxTrackIDMatch.hasMatch())? rxTrackIDMatch.captured(1).toInt() : -1; //-- Айдишник конкретного трека

  //-- Парсим остальные пришедшие строки
  QHash<QString, QString> dataParams; //-- список параметров параметр-значение
  QRegularExpression rxParams("([^:]*):\\s(.*)");

  while (dataList.length()>0) {
    QString paramStr =dataList.takeAt(0);
    QRegularExpressionMatch rxParamsMath =rxParams.match(paramStr);
    if ( rxParamsMath.hasMatch() ) {
      dataParams.insert(rxParamsMath.captured(1), rxParamsMath.captured(2));
    }
  }

  //-- Парсим параметр айдишник запроса
  int cseq =dataParams.value("CSeq", "-1").toInt();
  if ( cseq==-1 ) {
    qWarning()<<"Not set CSeq O_o, check request!";
    answer(400);
    return;
  }

  //-- Айдишник сессии
  uint32_t sessionId =dataParams.value("Session", "-1").toUInt();

  //== Обрабатываем запрос ==//

  if ( command=="OPTIONS" ) { //-- Спрашивают какие запросы мы можем обрабатывать
    answerOPTIONS(cseq);
    return;
  }

  //-- Проверим, залогинен ли пользователь, т.к. дальше без авторизации не обрабатываем никакие команды
  if ( !isActual() ) {
    if ( dataParams.contains("Authorization") ) { //-- Если в заголовках есть "Authorization:", значит нужно распарсить по нему
      QStringList authData = QString(QByteArray::fromBase64(dataParams.value("Authorization").mid(6, -1).toUtf8())).split(':');
      if ( authData.count()!=2 ) {
        qWarning()<<"Wrong auth data syntax!";
        answer(401, cseq);
        return;
      }
      if ( !loginUser(authData[0], authData[1]) ) {
        answer(401, cseq);
        return;
      }
    } else { //-- Иначе попробуем взять из URL
      if ( !loginUser(reqSource.userName(), reqSource.password()) ) {
        answer(401, cseq);
        return;
      }
    }
  }

  if ( command=="DESCRIBE" ) { //-- Спрашивают что у нас есть из камер (потоков)
    answerDESCRIBE(cseq, trackID);
    return;
  }

  if ( command=="SETUP" ) { //-- Настраивают поток
    QString transport =dataParams.value("Transport");
    QRegularExpression rxClientPorts("client_port=([\\d]*)-([\\d]*)");

    QRegularExpressionMatch rxClientPortsMatch = rxClientPorts.match(transport);
    if ( !rxClientPortsMatch.hasMatch() ) {
      qWarning()<<"Not set client_port";
      answer(400, cseq);
      return;
    }

    if ( trackID<0 ) {
      qWarning()<<"Not set track id! Check request!";
      answer(404, cseq);
      return;
    }

    //-- Каждый setup это своя сессия
    quint32 sessionId =generateSessionId();
    TSessionInfo sessionInfo;
    sessionInfo.trackId =trackID;
    sessionInfo.blockSize =dataParams.value("Blocksize", QString::number(_server->blockSize())).toUInt(); //-- Максимальный размер пакета, что бы влез в MTU, см. Server_Client_Streamer::onNewPacketAvaliable
    _sessions[sessionId] =sessionInfo;
    answerSETUP(cseq, rxClientPortsMatch.captured(1).toInt(), rxClientPortsMatch.captured(2).toInt(), sessionId);
    return;
  }

  if ( command=="PLAY" ) { //-- Запускают трансляцию потока
    answerPLAY(cseq, sessionId);
    return;
  }

  if ( command=="TEARDOWN" ) { //-- Завершают трансляцию потока
    answerTEARDOWN(cseq, sessionId);
    return;
  }

  if ( command=="ALIVE" ) { //-- Уведомляют, что им всё ещё нужна трансляция
    answerAlive(cseq, sessionId);
    return;
  }

  if ( command=="GET_PARAMETER" ) { //-- Запрашивают какой-то параметр
    answerGETPARAMETER(cseq, sessionId);
    return;
  }

  //TODO: Может быть стоит продолжить обработку _requestData, если commandEndIndex+8 не равен длинне?
}

/**
* @brief Формируем ответ об опциях
* @param cseq - номер запроса
*/
void Server_Client::answerOPTIONS(int cseq)
{
  QByteArray data;
  data.append("Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, ALIVE, GET_PARAMETER, SET_PARAMETER\r\n");
  answer(200, cseq, data);
}

/**
* @brief Формируем ответ клиенту об описании возможных камер
* @param cseq
* @param trackId - айдишник конкретного трека, или -1 когда не задан явно
*/
void Server_Client::answerDESCRIBE(int cseq, int trackId)
{
  SDP * sdp = dynamic_cast<SDP*>(_server->_cameras->getTotalSDP(trackId));
  sdp->origin.creatorName = "Cameron";
  sdp->origin.netType = "IN";
  sdp->origin.host = _server->_host;
  sdp->origin.sessionID = 38990265062388; //TODO: Выяснить, что сюда сувать
  sdp->origin.sessionVer = 38990265062388;

  QByteArray sdpData;
  if ( !sdp->make(sdpData) ) { //TODO: кэшировать
    qWarning()<<"SDP not maked!";
    answer(500, cseq);
    return;
  }

  int sdpSize = sdpData.size();

  sdpData.prepend("\r\n");
  sdpData.prepend(QString("Content-Length: %1\r\n").arg(sdpSize).toUtf8());
  sdpData.prepend("Content-Type: application/sdp\r\n");
  sdpData.prepend(QString("Content-Base: rtsp://%1:%2\r\n").arg(_server->_host.toString()).arg(_server->_port).toUtf8()); //-- Относительно этого адреса будут последующие запросы

  answer(200, cseq, sdpData, 0, false);
}

/**
* @brief Запускаем трансляцию
* @param cseq
* @param sessionId
*/
void Server_Client::answerPLAY(int cseq, uint32_t sessionId)
{
  qInfo()<<"Answer PLAY";
  if ( !checkSessionExists(sessionId) ) { qWarning()<<"Session not exists. Check request."; answer(404, cseq); return; }

  Server_Client_Streamer * streamer = _streamers.value(_sessions[sessionId].trackId);

  if ( streamer==nullptr ) {
    qWarning()<<"Streamer not exists!";
    answer(500, cseq);
    return;
  }

  if ( !streamer->start() ) {
    qWarning()<<"Streamer not started!";
    answer(500, cseq);
    return;
  }

  QByteArray data;  
  data.append(QString("Range: npt=0.000-").append("\r\n").toUtf8());

  answer(200, cseq, data, sessionId);
}

/**
* @brief Убиваем соединение и отвечаем
* @param cseq
* @param sessionId
*/
void Server_Client::answerTEARDOWN(int cseq, uint32_t sessionId)
{
  qDebug()<<"Answer TEARDOWN";
  if ( !checkSessionExists(sessionId) ) { qWarning()<<"Session not exists. Check request."; answer(404, cseq); return; }

  Server_Client_Streamer * streamer = _streamers.value(_sessions[sessionId].trackId);

  if ( streamer==nullptr ) {
    qWarning()<<"Streamer not exists!";
    answer(500, cseq);
    return;
  }

  streamer->stop();

  QByteArray data;
  answer(200, cseq, data, sessionId);

  _sessions[sessionId].trackId = -1;
}

/**
* @brief Готовим соединение
* @param cseq
* @param videoPort
* @param audioPort
* @param se
*/
void Server_Client::answerSETUP(int cseq, int videoPort, int audioPort, uint32_t sessionId)
{
  qInfo()<<"Answer SETUP"<<cseq<<"videoPort"<<videoPort<<"sessionId:"<<sessionId;

  const TSessionInfo &session =_sessions[sessionId];

  Cameras_Camera *camera =static_cast<Cameras_Camera*>(_server->_cameras->getCam(session.trackId));

  if ( camera==nullptr ){
    qWarning()<<"Camera with trackId"<<session.trackId<<"not found. Check request.";
    answer(404, cseq);
    return;
  }

  if ( !camera->go() ) {
    qWarning()<<"Unable start camera. Check settings.";
    answer(500, cseq);
    return;
  }

  Server_Client_Streamer *clientStreamer =new Server_Client_Streamer(this, _socket->peerAddress(), videoPort, camera->id(), camera->getStreamer());
  clientStreamer->setBlockSize(session.blockSize);

  connect(camera, &Cameras_Camera::errored, this, &Server_Client::onCameraErrored); //-- Если будут ошибки с камерой, нужно отреагировать
  connect(clientStreamer, &Server_Client_Streamer::destroyed, camera, &Cameras_Camera::stop); //-- Как только стример удалится - уведомляем камеру, что можно попробовать остановить вещание

  _streamers[clientStreamer->id()] =clientStreamer;

  QByteArray data;
  data.append(QString("Transport: RTP/AVP;unicast;destination=%1;source=%2;client_port=%3-%4\r\n").arg(
    _socket->peerAddress().toString(),
    _server->_host.toString(),
    QString::number(videoPort),
    QString::number(audioPort)).toUtf8()
  );

  answer(200, cseq, data, sessionId);
}

/**
* @brief Отвечаем на запрос хз чего хз как
* Некоторые плееры используют получение параметра как ALIVE запрос
* @param cseq
*/
void Server_Client::answerGETPARAMETER(int cseq, quint32 sessionId)
{
  qInfo()<<"GET_PARAMETER";
  if ( !checkSessionExists(sessionId) ) { qWarning()<<"Session not exists. Check request."; answer(404, cseq); return; }

  answerAlive(cseq, sessionId);
}

/**
* @brief Отлавливаем стример и удаляем из списка
* @param streamID
*/
void Server_Client::streamFinished(int streamID)
{
  qDebug()<<"";
  _streamers.remove(streamID);
}

/**
* @brief Отвечаем на запрос подтверждения, что клиент жив
* @param cseq
*/
void Server_Client::answerAlive(int cseq, uint32_t sessionId)
{
  qInfo()<<"ALIVE";
  if ( !checkSessionExists(sessionId) ) { qWarning()<<"Session not exists. Check request."; answer(404, cseq); return; }

  _aliveTimeOverTimer->start();
  QByteArray data;
  answer(200, cseq, data, sessionId);
}

/**
* @brief Отсылаем ответ клиенту
* @param statusCode - статус
* @param cseq - номер запроса
* @param data - сам ответ
* @param sessionId - айдишник сессии. Если не нужен, то -1.
* @param lastRN - нужно ли отправлять финальные \r\n
*/
void Server_Client::answer(int statusCode, int cseq, QByteArray data, uint32_t sessionId, bool lastRN)
{
  if ( statusCode==200 || statusCode==1 ) {
    data.prepend(QString("Server: Cameron\r\n").toUtf8());
    data.prepend(QString("CSeq: %1\r\n").arg(cseq).toUtf8());
    data.prepend("RTSP/1.0 200 OK\r\n");

    if ( sessionId>0 ) {
      data.append(QString("Session: %1;timeout=%2\r\n").arg(
        QString::number(sessionId),
        QString::number(_aliveTimeOverTimer->interval()/1000*0.8) //-- Интервал в секундах и возьмём с небольшим запасом
      ).toUtf8());
    }
  } else {
    data.clear();
    switch ( statusCode ) {
      case 401: { data.append("RTSP/1.0 401 Unauthorized\r\n"); break; }
      case 400: { data.append("RTSP/1.0 400 Bad Request\r\n"); break; }
      case 404: { data.append("RTSP/1.0 404 Not Found\r\n"); break; }
      case 500: { data.append("RTSP/1.0 500 Internal Server Error\r\n"); break; }
      default: { data.append("RTSP/1.0 400 Bad Request\r\n"); break; }
    }
    data.append(QString("Server: Cameron\r\n").toUtf8());
    data.append(QString("CSeq: %1\r\n").arg(cseq).toUtf8());
    if ( statusCode==401 ) { data.append(QString("WWW-Authenticate: Basic realm=\"Cameron\"\r\n").toUtf8()); }
  }

  if ( lastRN ) { data.append("\r\n"); }

  qInfo()<<"Answer to request "<<data;

  _socket->write(data);
}

/**
* @brief Таймер закончил отсчёт, клиент не прислал подтверждение, что он живой.
*/
void Server_Client::aliveTimeOver()
{
  qInfo()<<"";
  emit notAlive(_id);
}

/**
* @brief Создаём уникальный айдишник сессии
* @return
*/
quint32 Server_Client::generateSessionId()
{
  quint32 sessionId = 0;

  do {
    sessionId = QRandomGenerator::global()->generate();
  } while (_sessions.contains(sessionId));

  return sessionId;
}

/**
* @brief Проверяем, есть ли сессия
* @param sessionId
* @return
*/
bool Server_Client::checkSessionExists(quint32 sessionId)
{
  return _sessions.contains(sessionId);
}

/**
* @brief Попробуем залогинить пользователя
* @param uName
* @param uPass
* @return
*/
bool Server_Client::loginUser(QString uName, QString uPass)
{
  if ( isActual() ) { return true; }
  if ( (uName=="") || (uPass=="") ) { qWarning()<<"User name or password not set!"; return false; }
  QString uPassHash = _server->_avaliableUsers.value(uName, "");
  if ( !checkPass(uPass,uPassHash) ) { qWarning()<<"User name or password mismatch!"; return false; }
  if ( !setInfo(uName,uPassHash) ) { qWarning()<<"Unable login user!"; return false; }
  return true;
}

Server_Client::~Server_Client()
{
  qDebug()<<"";
}
