#ifndef RTSP_CLIENT_H
#define RTSP_CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QRegExp>
#include "Camera/cameras.h"
#include "Server/server_client_streamer.h"
#include <QApplication>

/**
 * @brief Подключившийся клиент
 * обрабатываем запросы,
 * отдаём инфу обо всех камерах
 */
class Server;
class Server_Client : public QObject
{
    Q_OBJECT
public:
    explicit Server_Client(QObject *parent = 0, QTcpSocket * socket = NULL, Server * server = NULL);
    ~Server_Client();
    int clientID();

signals:

public slots:
    void request();

    void streamFinished(int streamID);

private:
    QTcpSocket * _socket;
    Server * _server;

    void answerOPTIONS(int cseq);
    void answerDESCRIBE(int cseq);
    void answerPLAY(int cseq, int streamID);
    void answerTEARDOWN(int cseq, int streamID);
    void answerSETUP(int cseq, int videoPort, int audioPort, int camID);
    void answerGETPARAMETER(int cseq);
    void answer(bool status, int cseq =0, QByteArray data ="", bool lastRN =true);

    QList<Server_Client_Streamer*> _streamers; //-- список стримеров, которые запустил этот клиент

};

#endif // RTSP_CLIENT_H
