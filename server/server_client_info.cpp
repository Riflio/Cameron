#include "server_client_info.h"

Server_Client_Info::Server_Client_Info(QObject *parent) :
    QObject(parent), _name(""), _passHash(""), _actual(false)
{

}

QString Server_Client_Info::getName()
{
  return _name;
}

/**
* @brief Установлена вся нужная инфа или нет
* @return
*/
bool Server_Client_Info::isActual()
{
  return _actual;
}

/**
* @brief Устанавливаем информацию о пользователе
* @param name
* @param pass
*/
bool Server_Client_Info::setInfo(QString name, QString passHash)
{
  if ( name=="" || passHash=="" ) { return false; }
  _name = name;
  _passHash = passHash;
  _actual = true;
  return true;
}

/**
* @brief Проверяем соответствие пароля
* @param uPass - предлагаемый пароль
* @param passHash - правильный хэш пароля
* @return
*/
bool Server_Client_Info::checkPass(QString uPass, QString passHash)
{
  if ( passHash.isEmpty() ) { return false; }
  if ( passHash!=QCryptographicHash::hash(uPass.toUtf8(), QCryptographicHash::Md5).toHex()) { return false; }
  return true;
}
