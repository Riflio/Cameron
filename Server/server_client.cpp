#include "server_client.h"
#include "server.h"

#include <QDebug>

Server_Client::Server_Client( QObject *parent, QTcpSocket * socket, Server * server ):
    Server_Client_Info(parent), _socket(socket), _server(server)
{

    _id = _socket->socketDescriptor();

    connect(_socket, &QTcpSocket::readyRead, this, &Server_Client::request);

    _aliveTimeOverTimer = new QTimer(this);
    _aliveTimeOverTimer->setInterval(120000); //--
    connect(_aliveTimeOverTimer, &QTimer::timeout, this, &Server_Client::aliveTimeOver);

    _aliveTimeOverTimer->start(); //-- Клиент должен высылать подтверждение каждые 55 секунд, если с последнего запроса клиента о подтверждении прошло N времени, значит клиент отъехал нахуй.
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
    QString data = _socket->readAll();
    _requestData += data;

     qInfo()<<"\n\n\n"<<"New client request"<<data;

     int commandEndIndex = _requestData.indexOf("\r\n\r\n");

     if (commandEndIndex==-1) { //-- Команда полностью завершена только после пары \r\n, до этого момента накапливаем
         return;
     }

    //-- разбираем по строкам
    QStringList dataList = _requestData.split("\r\n");

    _requestData = "";

    //-- берём первую строку, узнаём команду, исходный юрл и версию ртсп
    QString initLine = dataList.takeAt(0);

    //-- парсим
    QRegExp rxInit("([A-Z_]*)\\s([^\\s]*)\\s(.*)");

    if ( rxInit.indexIn(initLine) == -1 ) { //-- пришла какая-то фигня, пропускаем
        qWarning()<<"Unable understand request!";
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
        answer(false);
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

        //-- Проверим имя пользователя и пароль
        QUrlQuery urlQuery(reqSource);

        QString uName = urlQuery.queryItemValue("user");
        QString uPass = urlQuery.queryItemValue("pass");

        if (uName=="" || uPass=="") {
            qWarning()<<"User name or password not set!";
            answer(false);
            return;
        }

        //-- Если инфы о пользователе ещё нет, то самое время узнать её
        if ( !isActual() ) {
            if ( !setInfo(uName, _server->_avaliableUsers.value(uName, "")) ) {
                qWarning()<<"User name not found on avalialbe users!";
                answer(false);
                return;
            }
        }

        if (!checkInfo(uName, uPass)) {
            qWarning()<<"User name or password mismatch!";
            answer(false);
            return;
        }

        answerOPTIONS(cseq);
        return;
    }

    if ( !isActual() ) return answer(false); //-- Дальше, если пользователь не авторизован не продолжаем

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
            qWarning()<<"Not set trackID!"<<reqSource.path();
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

     if (command=="ALIVE") {
         answerAlive(cseq);
         return;
     }

     if (command == "GET_PARAMETER") {
         answerGETPARAMETER(cseq);
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
    sdp->origin.sessionID=38990265062388; //TODO: Выяснить, что сюда сувать
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
    sdpData.prepend( "Content-Type: application/sdp\r\n" ); //-- В каком формате мы отдаём инфу, у нас sdp
    sdpData.prepend( QString("Content-Base: rtsp://%1:%2\r\n").arg(_server->_host.toString()).arg(_server->_port).toUtf8() ); //-- Относительно этого адреса будут последующие запросы

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

    if (!camera->go()) {
        qWarning()<<"Unable start camera";
        answer(false);
        return;
    }

    Server_Client_Streamer * clientStreamer = new Server_Client_Streamer(this, _socket->peerAddress(), videoPort, _streamers.length(), camera->getStreamer() );

    connect(camera, &Cameras_Camera::errored, this, &Server_Client::onCameraErrored); //-- Если будут ошибки с камерой, нужно отреагировать
    connect(clientStreamer, &Server_Client_Streamer::destroyed, camera, &Cameras_Camera::stop); //-- Как только клиент удалиться - останавливаем вещаение камеры

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
    qInfo()<<"GET_PARAMETER";
    answerAlive(cseq); //-- Некоторые используют получение параметра как ALIVE запрос...
}


/**
 * @brief Отлавливаем завершение стримера, и удаляем из списка
 * @param streamID
 */
void Server_Client::streamFinished(int streamID)
{
    qDebug()<<"";
    _streamers.takeAt(streamID); //-- удаляем из стримеров
}

/**
* @brief Отвечаем на запрос подтверждения, что клиент жив
* @param cseq
*/
void Server_Client::answerAlive(int cseq)
{
    qInfo()<<"ALIVE";
    _aliveTimeOverTimer->start(); //-- Запускаем заново таймер
    QByteArray data;
    answer(true, cseq, data);
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
        data.prepend(QString("Server: Cameron\r\n").toUtf8());
        data.prepend(QString("CSeq: %1\r\n").arg(cseq).toUtf8());
        data.prepend("RTSP/1.0 200 OK\r\n");
    } else {
        data.clear();
        data.append("RTSP/1.0 400 Bad Request\r\n");
        data.append(QString("Server: Cameron\r\n").toUtf8());
    }

    if (lastRN) { data.append("\r\n"); }

    qInfo()<<"Answer to request "<<data;

    _socket->write(data);
}

/**
* @brief Таймер закончил отсчёт, клиент не прислал подтверждение, что он живой, значит помер, удаляемся.
*/
void Server_Client::aliveTimeOver()
{
    qInfo()<<"";
    deleteLater();
}



Server_Client::~Server_Client()
{

    qDebug()<<"";

}

