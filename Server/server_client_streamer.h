#ifndef SERVER_CLIENT_STREAMER_H
#define SERVER_CLIENT_STREAMER_H

#include <QObject>
#include <QThread>
#include <QUdpSocket>

#include "Plugins/wthread.h"
#include "Camera/cameras_camera.h"

#include <QTimer>

/**
* @brief Отсылаем клиенту сервера фреймы
*/
class Server_Client_Streamer : public WThread
{
    Q_OBJECT
public:
    explicit Server_Client_Streamer(QObject * parent, QHostAddress host, int port, int id,  IRTSP_Stream * streamer);
    ~Server_Client_Streamer();

    void loop() override;
    bool onStarted() override;

    int id();


private:
    QHostAddress _host;
    int _port;
    int _id;

    IRTSP_Stream * _streamer;
    QUdpSocket * _socket;

    long long int _buffOffset;

};

#endif // SERVER_CLIENT_STREAMER_H
