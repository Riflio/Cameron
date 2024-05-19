#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>

#include "interfaces/iqcameron.h"
#include "settings/settings.h"
#include "camera/cameras.h"
#include "server/server.h"
#include "plugins/pluginsmanager.h"

class AppCore: public IQCameron
{
  Q_OBJECT
public:
  explicit AppCore(QObject *parent =nullptr);
  ~AppCore();

  static void myMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString &msg);

  const QString VERSION ="1.1.0";

  QString version() const override;
  IServer *server() const override;
  ISettings *settings() const override;
  IPluginsManager *pluginsManager() const override;

signals:

public slots:
  bool appStart();

private:
  Settings *_settings =nullptr;
  Server *_server =nullptr;
  Cameras *_cameras =nullptr;
  PluginsManager *_pluginsManager =nullptr;

};

#endif // APPCORE_H
