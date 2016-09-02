#include "appcore.h"
#include <QDebug>

AppCore::AppCore(QObject *parent) : QObject(parent)
{
    _settings = new Settings(this);

    _settings->load(_cams);

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
