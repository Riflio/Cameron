#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QFile>
#include <QXmlStreamReader>

#include "camera.h"
#include "db/db.h"


class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);
    bool load(TCams & cams, DB * db);




signals:

public slots:

private:
    QFile* _settingsFile;

};

#endif // SETTINGS_H
