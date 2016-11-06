#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QDebug>


class PluginInterface
{

public:
    virtual ~PluginInterface() {}

    virtual void Init() =0;

    QVariant ARG(void * p) { return qVariantFromValue((void *)p); }



public slots:


signals:
    void doAction(QString, QVariantList);
    void addAction(QString, QObject *, const char *);
};

Q_DECLARE_INTERFACE(PluginInterface, "com.Cameron.Application.PluginInterface/1.0")

#endif // PLUGININTERFACE_H
