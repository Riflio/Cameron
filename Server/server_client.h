#ifndef RTSP_CLIENT_H
#define RTSP_CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QRegExp>
#include "Camera/cameras.h"

/**
 * @brief Подключившийся клиент
 */
class Server;
class Server_Client : public QObject
{
    Q_OBJECT
public:
    explicit Server_Client(QObject *parent = 0, QTcpSocket * socket = NULL, Server * server = NULL);

    int clientID();

signals:

public slots:
    void request();

private:
    QTcpSocket * _socket;
    Server * _server;

    void answerOPTIONS(int cseq);
    void answerDESCRIBE(int cseq);
    void answerPLAY(int cseq);
    void answerTEARDOWN(int cseq);
    void answerSETUP(int cseq);    
    void answer(bool status, int cseq =0, QByteArray data ="", bool lastRN =true);

};

#endif // RTSP_CLIENT_H
