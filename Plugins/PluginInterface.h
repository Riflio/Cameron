#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QObject>
#include <QString>
#include <QVariant>


class PluginInterface
{
public:
  virtual ~PluginInterface() {}

  virtual void Init() =0;

  virtual void InstallPlugin() =0;
  virtual void UnInstallPlugin() =0;

  QVariant ARG(void * p) { return QVariant::fromValue((void *)p); }

public slots:

signals:
  void doAction(QString, QVariantList);
  void addAction(QString, QObject *, const char *, Qt::ConnectionType);
};

Q_DECLARE_INTERFACE(PluginInterface, "com.Cameron.Application.PluginInterface/1.0")

#endif // PLUGININTERFACE_H
