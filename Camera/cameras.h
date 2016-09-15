#ifndef CAMERAS_H
#define CAMERAS_H

#include <QObject>

class Cameras : public QObject
{
    Q_OBJECT
public:
    explicit Cameras(QObject *parent = 0);

signals:

public slots:
};

#endif // CAMERAS_H