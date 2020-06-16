#ifndef SERVER_CLIENT_INFO_H
#define SERVER_CLIENT_INFO_H

#include <QObject>
#include <QCryptographicHash>

/**
* @brief Информация о подключившемся пользователе
*/
class Server_Client_Info : public QObject
{
    Q_OBJECT
public:
    explicit Server_Client_Info(QObject *parent =nullptr);

signals:

public slots:
    bool checkPass(QString uPass, QString pass);
    bool setInfo(QString name, QString passHash);
    QString getName();
    bool isActual();

private:
    QString _name;
    QString _passHash;
    bool _actual;
};

#endif // SERVER_CLIENT_INFO_H
