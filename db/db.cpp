#include "db.h"
#include <QDebug>

DB::DB(QObject *parent) : QObject(parent)
{

}

void DB::setSettings(QString host, QString db, QString user, QString pass)
{
    qInfo()<<"DB setSettings"<<host<<db<<user<<pass;

    _host = host;
    _db = db;
    _user = user;
    _pass = pass;
}

bool DB::connectDB()
{
    qInfo()<<"Connect DB";

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName(_host);
    db.setPort(3306);
    db.setUserName(_user);
    db.setPassword(_pass);
    db.setDatabaseName(_db);

    return db.open();

}
