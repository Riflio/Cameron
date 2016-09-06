#include "appcore.h"
#include <QDebug>

AppCore::AppCore(QObject *parent) : QObject(parent)
{
    _settings = new Settings(this);

    _db = new DB(this);

    _settings->load(_cams, _db);

    //-- коннектимся к базе
    qInfo()<< "status "<<_db->connectDB();

    //-- стартуем все камеры
    QListIterator<Camera*> i(_cams);
    while (i.hasNext()) {
        i.next()->start();
    }


}

AppCore::~AppCore()
{
    qDebug()<<"DELETE APPCORE";

}
