#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>
#include "settings/settings.h"

class AppCore : public QObject
{
    Q_OBJECT
public:
    explicit AppCore(QObject *parent = 0);
    ~AppCore();
signals:

public slots:

private:
    Settings * _settings;
    TCams _cams;
};

#endif // APPCORE_H
