#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

#include <QObject>

class Server_Client : public QObject
{
    Q_OBJECT
public:
    explicit Server_Client(QObject *parent = 0);

signals:

public slots:
};

#endif // SERVER_CLIENT_H