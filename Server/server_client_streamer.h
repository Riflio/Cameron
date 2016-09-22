#ifndef SERVER_CLIENT_STREAMER_H
#define SERVER_CLIENT_STREAMER_H

#include <QObject>
#include <QThread>
#include <QUdpSocket>
#include <QApplication>

#include "Camera/cameras_camera.h"

/**
 * @brief Запускаем камеру, берём фреймы и отсылает по указанным host и port
 */
class Server_Client_Streamer : public QObject
{
    Q_OBJECT
public:
    explicit Server_Client_Streamer(QObject * parent, QHostAddress host, int port, int id,  Cameras_Camera * cam);
    ~Server_Client_Streamer();

    void process();

    Cameras_Camera * cam();

    int id();

signals:
    void finished(int id);

public slots:
    bool start();
    void stop();

private:
    QUdpSocket * _socket;

    QHostAddress _host;
    int _port;
    int _id;

    QThread * _thread;
    Cameras_Camera * _cam;


    bool _started;

    long long int _buffOffset;
};

#endif // SERVER_CLIENT_STREAMER_H
