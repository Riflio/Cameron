#include "rtsp.h"
namespace NS_RSTP {

RTSP::RTSP(QObject *parent) : QObject(parent), _reqID(0)
{
    _sckConnect = new QTcpSocket(nullptr); //-- Без предка, потому что нам нужно самим в нужной последовательности всё удалить

    _sdp = new SDP(this);

    connect(_sckConnect, &QTcpSocket::readyRead, this, &RTSP::onSckConnectReadyRead);
    connect(_sckConnect, &QTcpSocket::connected, this, &RTSP::onSckConnectConnected);
    connect(_sckConnect, &QTcpSocket::disconnected, this, &RTSP::onSckConnectDisconnected);
    connect(_sckConnect, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &RTSP::onSckConnectError);
}


/**
* @brief 1.1 Коннектимся к камере
* @param url
*/
void RTSP::cameraConnect(QHostAddress address, int port, QString params)
{
    qInfo()<<"RTSP cameraConnect"<<address<<port;

    _gateway = QString("rtsp://%1:%2/%3").arg(address.toString()).arg(port).arg(params);
    _sckConnect->connectToHost(address, port);
}

void RTSP::cameraConnect(QString url)
{
    _gateway = url;
    QUrl decodeURL = QUrl(url);

    qInfo()<<"RTSP cameraConnect url"<< url<<decodeURL.host()<<decodeURL.port();

    _sckConnect->connectToHost( decodeURL.host(), decodeURL.port() );
}

/**
* @brief 1.2 Подконнектится получилось, начинаем общение
*/
void RTSP::onSckConnectConnected()
{
    qInfo()<<"Camera socket connected";
    options();
}

/**
* @brief 2. Запросим камеру о доступных командах
*/
void RTSP::options()
{
    qInfo()<<"RTSP options";

    SendParams params;
    params.insert("User-Agent", "QRTSP");

    int r = send(OPTIONS, params);

    reqHistories.insert(r, ReqHistory(OPTIONS, -1));
}

/**
* @brief 3 Запросим камеру о доступных каналах и их свойствах
*/
void RTSP::describe()
{
    qInfo()<<"RTSP describe";

    SendParams params;
    int r = send(DESCRIBE, params);

    reqHistories.insert(r, ReqHistory(DESCRIBE, -1));
}

/**
* @brief 4. Создаём канал у камеры и выставляем настройки
* @param channel
* @param port
*/
void RTSP::setup(int channel, int port)
{
    qInfo()<<"RTSP setup"<<channel<<port;

    SendParams params;
    params.insert("Transport", QString("RTP/AVP;unicast;client_port=%1-%2").arg(port).arg(port+1) );

    int r =send(SETUP, params);

    reqHistories.insert(r, ReqHistory(SETUP, channel));
}


/**
* @brief 5. Отсылаем запрос на начало воспроизведения
* @param channel
*/
void RTSP::play(int channel)
{
    qInfo()<<"RTSP play"<<channel;

    SendParams params;
    params.insert("Session", QString::number( _channels.at(channel)->session()));
    params.insert("Range", "npt=0.000-");

    int r = send(PLAY, params);

    reqHistories.insert(r, ReqHistory(PLAY, channel));
}

/**
* @brief Через заданные промежутки времени отсылаем запрос, что мы живы
*/
void RTSP::alive(int channel)
{
    SendParams params;
    params.insert("Session", QString::number( _channels.at(channel)->session()));

    int r = send(ALIVE, params);
    reqHistories.insert(r, ReqHistory(ALIVE, channel));
}

/**
* @brief 7. Отсылаем запрос на завершение воспроизведения
* @param channel
*/
void RTSP::teardown(int channel)
{
    qInfo()<<"RTSP teardown"<<channel;

    emit toTeardown(channel);

    SendParams params;
    params.insert("Session", QString::number( _channels.at(channel)->session()));
    int r = send(TEARDOWN, params);

    reqHistories.insert(r, ReqHistory(TEARDOWN, channel));

}

/**
* @brief Пришёл ответ на запрос от камеры
*
*/
void RTSP::onSckConnectReadyRead()
{
    /**
     * Ответ может состоять из двух частей всё, что до пустой строки будем считать заголовком,
     * всё, что после - дополнительными данными.
     * Их парсинг зависит уже от того, что в заголовке
     */

    QString responseText = _sckConnect->readAll();

    qInfo()<<"RTSP answer"<<responseText;

    //-- Разделяем построчно
    QStringList responseList = responseText.split("\r\n");

    //-- В первой строке - статус
    if ( responseList.takeAt(0)!="RTSP/1.0 200 OK" ) {
        qWarning()<<"ERROR";
        return;
    }

    //-- Разбираем строки ответа на составляющие
    struct responseData {
        QString command;
        QString value;
        QString params;
    };
    QMap<QString, responseData> responseDataList;

    QString extraData=""; //-- Дополнительные данные

    //-- Нужно узнать на какой запрос этот ответ
    ReqHistory historyReq;

    //-- Команда-всё, что до ":" значение - всё, что после и до первой ";" а дальше дополнительные параметры, если есть.
    QRegExp rx("(.+):\\s?([^;]*);?(.*)?", Qt::CaseInsensitive, QRegExp::RegExp2);

    //-- Парсим до появления пустой строки
    while (responseList.length()>0) {

        QString responseStr = responseList.takeAt(0);

        if ( responseStr.isEmpty() ) { break; }

        if ( rx.indexIn(responseStr)>-1 ) {

            responseData data;
            data.command = rx.cap(1);
            data.value = rx.cap(2);
            data.params = rx.cap(3);

            responseDataList.insert(data.command, data);

            //-- Пока разбираем весь ответ заодно ищем айдишник запроса, что бы узнать на какой хоть запрос этот ответ
            if ( data.command=="Cseq" ) {
                historyReq = reqHistories.value(data.value.toInt());
            }
        }
    }

    //-- Заголовок разобрали, оставшиеся строки соединим обратно и засунем в дополнительные данные
    extraData = responseList.join("\r\n");

    //-- Так, запрос узнали, ответ разобрали, решаем, что с ним делать
    switch (historyReq.method) {

        case OPTIONS:{
            describe();
            break;
        }

        case DESCRIBE: {
            //-- Парсим каналы

            //-- Распарсим sdp данные
            _sdp->parse(extraData.toUtf8());

            qDebug()<<"SDP channels count"<<_sdp->medias.length();

            for (int i = 0; i < _sdp->medias.length(); ++i) {
                RTSP_Channel * channel = new RTSP_Channel(nullptr, this); //-- Без предка, потому что самостоятельно будем удалять, т.к. они могут перед удалением что-нить отправить

                //-- Отлавливаем события
                connect(channel, &RTSP_Channel::errored, this, &RTSP::errored);

                channel->_sdpMedia = _sdp->medias.at(i);
                _channels.append(channel);
            }

            emit connected();
            break;
        }

        case SETUP: {
            RTSP_Channel * channel = getChannel(historyReq.channel);

            //-- От ответа, пока что, нам нужен номер сессии
            channel->_session = responseDataList.value("Session").value.toInt();

            emit setuped(channel->id());
            break;
        }

        case PLAY: {
            emit played(historyReq.channel);
            break;
        }

        case TEARDOWN: {
            emit teardowned(historyReq.channel);
            break;
        }

        case GET_PARAMETER:
        case ALIVE: {
            RTSP_Channel * channel = getChannel(historyReq.channel);
            channel->alived();
            emit alived(channel->id());
        }

        case NONE: {
            break;
        }

    }
}


/**
* @brief Подготавливаем и отправляем запрос к камере
* @param method
* @param params
*/
int RTSP::send(METHODS method, SendParams params)
{
    QString sMethod;
    switch (method) {
        case OPTIONS: sMethod="OPTIONS"; break;
        case DESCRIBE: sMethod="DESCRIBE"; break;
        case SETUP: sMethod="SETUP"; break;
        case PLAY: sMethod="PLAY"; break;
        case GET_PARAMETER: sMethod="GET_PARAMETER"; break;
        case TEARDOWN: sMethod="TEARDOWN"; break;
        case ALIVE: sMethod="GET_PARAMETER"; break; //-- Не хотят некоторые камеры воспринимать ALIVE, а вот GET_PARAMETER проходит
        default: break;
    }

    _reqID++;

    //-- Запишем базовую необходимую инфу
    _sckConnect->write(QString("%1 %2 RTSP/1.0\r\n").arg(sMethod).arg(_gateway).toUtf8());
    _sckConnect->write(QString("CSeq: %1\r\n").arg(_reqID).toUtf8());

    //-- Пробегаемся по всем параметрам и добавляем к запросу
    SendParams::iterator i;
    for (i = params.begin(); i != params.end(); ++i) {
        _sckConnect->write( QString("%1: %2\r\n").arg(i.key()).arg(i.value()).toUtf8() );
    }

    //-- Заканчиваем пустой строкой
    _sckConnect->write("\r\n");

    return _reqID;
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
RTSP_Channel * RTSP::getChannel(int id)
{
    return (id<channelsCount()) ? _channels[id] : NULL;
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
    qDeleteAll(_channels);
    _channels.clear();
    _sckConnect->deleteLater();
}

}
