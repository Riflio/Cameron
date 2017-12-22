#ifndef SERVER_CLIENT_STREAMER_H
#define SERVER_CLIENT_STREAMER_H

#include <QObject>
#include <QThread>
#include <QUdpSocket>
#include <QApplication>

#include "Assets/wthread.h"
#include "Camera/cameras_camera.h"

#include <QTimer>

/**
 * @brief Запускаем камеру, берём фреймы и отсылает по указанным host и port
 */
class Server_Client_Streamer : public WThread
{
    Q_OBJECT
public:
    explicit Server_Client_Streamer(QObject * parent, QHostAddress host, int port, int id,  IRTSP_Stream * streamer);
    ~Server_Client_Streamer();

    void process();

    int id();


private:
    QHostAddress _host;
    int _port;
    int _id;

    IRTSP_Stream * _streamer;
    QUdpSocket * _socket;

    long long int _buffOffset;

    QTimer * _processTimer;
    void  processLoop();
};

#endif // SERVER_CLIENT_STREAMER_H
