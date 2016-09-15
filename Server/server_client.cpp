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
    qInfo()<<"New request read";

    QByteArray data = _socket->readAll();

    qInfo()<< data;

    QRegExp rx;

    //-- парсим сам запрос
    rx.setPattern("^([A-Z]*)\\s(.*)\\s(RTSP/1.0)");

    if ( rx.indexIn(data) == -1 ) { //-- пришла какая-то фигня, пропускаем
        qWarning()<<"Unable undestand request!";
        answer(false);
        return;
    }

    QString command = rx.cap(1);

    //-- парсим его номер
    rx.setPattern("CSeq:\\s([0-9]{1,})");
    if ( rx.indexIn(data,0) == -1 ) { //-- не задан номер, пропускаем
        qWarning()<<"Not set CSeq";
        answer(false);
        return;
    }

    int cseq = rx.cap(1).toInt();

    //-- Обрабатываем запрос
    if (command=="OPTIONS") {
        answerOPTIONS(cseq);
        return;
    }

    if (command=="DESCRIBE") {
        answerDESCRIBE(cseq);
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

    SDP * sdp = _server->_cameras->getTotalSDP();
    sdp->origin.creatorName="Cameron";
    sdp->origin.netType="IN";
    sdp->origin.host=_server->_host;
    sdp->origin.sessionID=38990265062388;
    sdp->origin.sessionVer=38990265062388;

    SDP::sAttribute * atrRange = new SDP::sAttribute();
    atrRange->name="range";
    atrRange->value="npt=0-";
    sdp->origin.attribytes.insert("range", atrRange);

    SDP::sAttribute * artControl = new SDP::sAttribute();
    artControl->name="control";
    artControl->value="*";
    sdp->origin.attribytes.insert("control", artControl);

    QByteArray sdpData;
    if (!sdp->make(sdpData)) { //TODO: кэшировать
        answer(false);
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
void Server_Client::answerPLAY(int cseq)
{

}

/**
 * @brief Убиваем соединение и отвечаем
 * @param cseq
 */
void Server_Client::answerTEARDOWN(int cseq)
{

}

/**
 * @brief Готовим соединение и отвечаем о готовности
 * @param cseq
 */
void Server_Client::answerSETUP(int cseq)
{
    //-- выносим в отдельный  поток что бы основной не забивать
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
        data = "FAIL"; //TODO: Узнать, что нужно отвечать
    }

    if (lastRN) { data.append("\r\n"); }

    qInfo()<<"Answer "<<data;

    _socket->write(data);
}



