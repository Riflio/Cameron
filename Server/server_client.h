#ifndef RTSP_CLIENT_H
#define RTSP_CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QRegExp>
#include <QUrlQuery>
#include "Camera/cameras.h"
#include "Server/server_client_streamer.h"
#include "server_client_info.h"
#include <QApplication>

/**
 * @brief Подключившийся клиент
 * обрабатываем запросы,
 * отдаём инфу обо всех камерах
 */
class Server;
class Server_Client : public Server_Client_Info
{
    Q_OBJECT
public:
    explicit Server_Client(QObject *parent = 0, QTcpSocket * socket = NULL, Server * server = NULL);
    ~Server_Client();


signals:

public slots:
    void request();
    void streamFinished(int streamID);
    int clientID();

private slots:
    void onCameraErrored();

private:
    QTcpSocket * _socket;
    Server * _server;
    int _id;

    void answerAlive(int cseq);
    void answerOPTIONS(int cseq);
    void answerDESCRIBE(int cseq);
    void answerPLAY(int cseq, int streamID);    
    void answerTEARDOWN(int cseq, int streamID);
    void answerSETUP(int cseq, int videoPort, int audioPort, int camID);
    void answerGETPARAMETER(int cseq);
    void answer(bool status, int cseq =0, QByteArray data ="", bool lastRN =true);

    QList<Server_Client_Streamer*> _streamers; //-- список стримеров, которые запустил этот клиент

    void aliveTimeOver();
    QTimer * _aliveTimeOverTimer;

    QString _requestData;
};

#endif // RTSP_CLIENT_H
