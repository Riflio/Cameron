#include "server_client_info.h"

Server_Client_Info::Server_Client_Info(QObject *parent) :
    QObject(parent), _name(""), _pass(""), _actual(false)
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
bool Server_Client_Info::setInfo(QString name, QString pass)
{
    if (name=="" || pass=="")  return false;
    _name = name;
    _pass = pass;
    _actual = true;

    return true;
}

bool Server_Client_Info::checkInfo(QString name, QString pass)
{
    if (name=="" || pass=="")  return false;

    if (_name != name) return false;
    if (_pass != QCryptographicHash::hash(pass.toUtf8(), QCryptographicHash::Md5).toHex()) return false;

    return true;
}
