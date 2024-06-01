#ifndef CAMERON_RIP_FILES_H
#define CAMERON_RIP_FILES_H

#include <QObject>
#include <QHash>
#include <QXmlStreamReader>

#include "interfaces/iplugininterface.h"
#include "interfaces/icameronrecorder.h"
#include "interfaces/iqcameron.h"
#include "interfaces/iqsettings.h"
#include "interfaces/iserver.h"
#include "recorder.h"

#include "recordersettings.h"

/**
* @brief Дополнение для Cameron - видеорегистратор
*/
class CameronRecorder: public QObject, public IPluginInterface, public ICameronRecorder
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "com.Cameron.Plugin.CameronRecorder/1.0" FILE "cameronrecorder.json" )
  Q_INTERFACES(IPluginInterface)

public:
  bool init(ICameron *cameron) override;
  void deInit() override;
  void installPlugin() override;
  void unInstallPlugin() override;

  IRecorder *getRecorder(int id) const override;

signals:

public slots:

private slots:
  void onCameronStarted();
  void onLoadingSettings(QString name, const QXmlStreamReader *xml);
  void onServerStarted(IServer *server);

private:
  ICameron *_cameron =nullptr;
  QHash<int, TRecorderSettings> _settings;
  QHash<int, Recorder*> _recorders;

};

#endif // CAMERON_RIP_FILES_H
