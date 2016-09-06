#ifndef DB_H
#define DB_H

#include <QObject>
#include <QSqlDatabase>

class DB : public QObject
{
    Q_OBJECT
public:
    explicit DB(QObject *parent = 0);
    bool connectDB();
    void setSettings(QString host, QString db, QString user, QString pass);
signals:

public slots:

private:
    QString _host;
    QString _db;
    QString _user;
    QString _pass;
};

#endif // DB_H
