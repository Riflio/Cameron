#include "server_client.h"
#include "server.h"

#include <QDebug>

Server_Client::Server_Client( QObject *parent, QTcpSocket * socket, Server * server ):
    QObject(parent), _socket(socket), _server(server)
{

}

int Server_Client::clientID()
{
    return _socket->socketDescriptor();
}


/**
 * @brief Есть новый запрос
 */
void Server_Client::request()
{
    QString data = _socket->readAll();

     qInfo()<<"New client request"<< data;

    //-- разбираем по строкам
    QStringList dataList = data.split("\r\n");

    //-- берём первую строку, узнаём команду, исходный юрл и версию ртсп
    QString initLine = dataList.takeAt(0);

    //-- парсим
    QRegExp rxInit("([A-Z]*)\\s([^\\s]*)\\s(.*)");

    if ( rxInit.indexIn(initLine) == -1 ) { //-- пришла какая-то фигня, пропускаем
        qWarning()<<"Unable undestand request!";
        answer(false);
        return;
    }

    if ( rxInit.cap(3)!="RTSP/1.0") {
        qWarning()<<"Wrong RTSP version O_o, check request!";
        answer(false);
        return;
    }

    QString command = rxInit.cap(1);
    QUrl reqSource = QUrl(rxInit.cap(2));

    if (! reqSource.isValid() ) {
        qWarning()<<"Request source URL invalid!";
        return;
    }


    //-- парсим остальные пришедшие строки
    QHash<QString, QString> dataParams; //-- список параметров параметр-значение
    QRegExp rxParams("([^:]*):\\s(.*)");

    while (dataList.length()>0) {
        QString paramStr = dataList.takeAt(0);
        if (rxParams.indexIn(paramStr)>-1) {
            dataParams.insert(rxParams.cap(1), rxParams.cap(2));
        }
    }

    //-- получаем общие параметры
    int cseq = dataParams.value("CSeq", "-1").toInt();
    if (cseq==-1) {
        qWarning()<<"Not set CSeq O_o, check request!";
        answer(false);
        return;
    }


    //-- Обрабатываем запрос
    if (command=="OPTIONS") {
        answerOPTIONS(cseq);
        return;
    }

    if (command=="DESCRIBE") {
        answerDESCRIBE(cseq);
        return;
    }

    if (command=="SETUP") {

        QString transport = dataParams.value("Transport");
        QRegExp rxClientPorts("client_port=([\\d]*)-([\\d]*)");

        if (rxClientPorts.indexIn(transport)==-1) {
            qWarning()<<"No set client_port";
            answer(false);
            return;
        }

        QRegExp rxCamID("trackID=([\\d]*)");
        if (rxCamID.indexIn(reqSource.path())==-1) {
            qWarning()<<"Not set trackID!";
            answer(false);
            return;
        }


        answerSETUP(cseq, rxClientPorts.cap(1).toInt(), rxClientPorts.cap(2).toInt(), rxCamID.cap(1).toInt() );

        return;
    }

     if (command=="PLAY") {
        answerPLAY( cseq, dataParams.value("Session", "-1").toInt() );
        return;
     }

     if (command == "TEARDOWN") {
         answerTEARDOWN(cseq, dataParams.value("Session", "-1").toInt() );
         return;
     }

     if (command == "GET_PARAMETER") {
         answerGETPARAMETER(cseq);
         return;
     }

}


/**
 * @brief Формируем ответ об опциях
 * @param cseq - номер запроса
 */
void Server_Client::answerOPTIONS(int cseq)
{
    QByteArray data;
    data.append("Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, GET_PARAMETER, SET_PARAMETER\r\n");

    answer(true, cseq, data);
}

/**
 * @brief Формируем ответ клиенту об описании возможных камер
 * @param cseq
 */
void Server_Client::answerDESCRIBE(int cseq)
{

    SDP * sdp = dynamic_cast<SDP*>(_server->_cameras->getTotalSDP());
    sdp->origin.creatorName="Cameron";
    sdp->origin.netType="IN";
    sdp->origin.host=_server->_host;
    sdp->origin.sessionID=38990265062388;
    sdp->origin.sessionVer=38990265062388;



    QByteArray sdpData;
    if (!sdp->make(sdpData)) { //TODO: кэшировать
        qWarning()<<"SDP not maked!";
        answer(false);
        return;
    }

    int sdpSize = sdpData.size();
    sdpData.prepend( "\r\n" );    
    sdpData.prepend( QString("Content-Length: %1\r\n").arg( sdpSize ).toUtf8() );
    sdpData.prepend( "Content-Type: application/sdp\r\n" );



    answer(true, cseq, sdpData, false);
}

/**
 * @brief Запускаем трансляции и отвечаем
 * @param cseq
 */
void Server_Client::answerPLAY(int cseq, int streamID)
{
    qInfo()<<"Answer PLAY";

    if ( streamID>_streamers.length() || streamID<0 ) {
        qWarning()<<"Streamer not found O_o";
        answer(false);
        return;
    }

    Server_Client_Streamer * streamer = _streamers.at(streamID);

    if ( streamer==NULL ) {
        qWarning()<<"Streamer was deleted O_O";
        answer(false);
        return;
    }

    if (!streamer->start()) {
        qWarning()<<"Streamer not started!";
        answer(false);
        return;
    }

    QByteArray data;

    data.append(QString("Session: %1").arg(streamer->id()).append("\r\n"));
    data.append(QString("Range: npt=0.000-").append("\r\n"));

    answer(true, cseq, data);

}

/**
 * @brief Убиваем соединение и отвечаем
 * @param cseq
 */
void Server_Client::answerTEARDOWN(int cseq, int streamID)
{
    qDebug()<<"Answer TEARDOWN";

    if (streamID<0 || streamID > _streamers.length()) {
        qWarning()<<"Stream not found O_o";
        answer(false);
        return;
    }

    Server_Client_Streamer * streamer = _streamers.at(streamID);

    if (streamer==NULL) {
        qWarning()<<"Stream deleted O_O";
        answer(false);
        return;
    }

    streamer->stop();

    QByteArray data;
    answer(true, cseq, data);
}

/**
 * @brief Готовим соединение и отвечаем о готовности
 * @param cseq
 */
void Server_Client::answerSETUP(int cseq, int videoPort, int audioPort, int camID)
{
    qInfo()<<"Answer SETUP"<<cseq<<videoPort<<camID;

    Cameras_Camera * camera = static_cast<Cameras_Camera*>( _server->_cameras->getCam(camID) );

    if (camera == NULL ){
        qWarning()<<"Camera not found O_o";
        answer(false);
        return;
    }


    Server_Client_Streamer * clientStreamer = new Server_Client_Streamer(this, _socket->peerAddress(), videoPort, _streamers.length(), camera );

    _streamers.append(clientStreamer);

    QByteArray data;

    data.append(QString("Transport: RTP/AVP;unicast;destination=%1;source=%2;client_port=%3-%4").arg(_socket->peerAddress().toString()).arg(_server->_host.toString()).arg(videoPort).arg(audioPort).append("\r\n") );
    data.append(QString("Session: %1").arg(clientStreamer->id()).append("\r\n")); //-- в сессию поставим айдишник, что бы знать в дальнейшем каким потоком управляет клиент

    answer(true, cseq, data);
}


/**
 * @brief Отвечаем на запрос хз чего хз как
 * @param cseq
 */
void Server_Client::answerGETPARAMETER(int cseq)
{
    qInfo()<<"GET PARAMETER";
    QByteArray data;
    answer(true, cseq, data);
}


/**
 * @brief Отлавливаем завершение стримера, и удаляем из списка
 * @param streamID
 */
void Server_Client::streamFinished(int streamID)
{
    _streamers.takeAt(streamID); //-- удаляем из стримеров
}


/**
 * @brief Отсылаем ответ клиенту
 * @param cseq - номер запроса
 * @param data - сам ответ
 * @param lastRN - нужно ли отправлять финальные \r\n
 */
void Server_Client::answer(bool status, int cseq, QByteArray data, bool lastRN)
{
    if (status) {
        data.prepend(QString("CSeq: %1\r\n").arg(cseq).toUtf8());
        data.prepend("RTSP/1.0 200 OK\r\n");
    } else {
        data.clear();
        data.append("RTSP/1.0 400 Bad Request\r\n");
    }

    if (lastRN) { data.append("\r\n"); }

    qInfo()<<"Answer to request "<<data;

    _socket->write(data);
}

Server_Client::~Server_Client()
{
    qInfo()<<"ServerClient deleted";

}

