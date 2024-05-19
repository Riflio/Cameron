#include "serverclientinfo.h"

ServerClientInfo::ServerClientInfo(QObject *parent): QObject(parent)
{

}

QString ServerClientInfo::getName() const
{
  return _name;
}

/**
* @brief Установлена вся нужная инфа или нет
* @return
*/
bool ServerClientInfo::isActual() const
{
  return _actual;
}

/**
* @brief Устанавливаем информацию о пользователе
* @param name
* @param pass
*/
bool ServerClientInfo::setInfo(QString name, QString passHash)
{
  if ( name=="" || passHash=="" ) { return false; }
  _name =name;
  _passHash =passHash;
  _actual =true;
  return true;
}

/**
* @brief Проверяем соответствие пароля
* @param uPass - предлагаемый пароль
* @param passHash - правильный хэш пароля
* @return
*/
bool ServerClientInfo::checkPass(QString uPass, QString passHash)
{
  if ( passHash.isEmpty() ) { return false; }
  if ( passHash!=QCryptographicHash::hash(uPass.toUtf8(), QCryptographicHash::Md5).toHex() ) { return false; }

  return true;
}
