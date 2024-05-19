#ifndef SERVERCLIENTINFO_H
#define SERVERCLIENTINFO_H

#include <QObject>
#include <QCryptographicHash>
#include "interfaces/iserverclientinfo.h"

/**
* @brief Информация о подключившемся пользователе
*/
class ServerClientInfo: public QObject, public virtual IServerClientInfo
{
  Q_OBJECT
public:
  explicit ServerClientInfo(QObject *parent =nullptr);

signals:

public slots:
  bool checkPass(QString uPass, QString pass);
  bool setInfo(QString name, QString passHash);
  QString getName() const override;
  bool isActual() const override;

private:
  QString _name;
  QString _passHash;
  bool _actual =false;
};

#endif // SERVERCLIENTINFO_H
