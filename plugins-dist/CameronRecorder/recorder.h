#ifndef RIPPERTHREAD_H
#define RIPPERTHREAD_H

#include <QObject>
#include <QFile>
#include <QList>
#include <QTimer>
#include <QThread>

#include "interfaces/irecorder.h"
#include "interfaces/recfileinfo.h"
#include "recordersettings.h"
#include "interfaces/icamera.h"

#include "recorderthread.h"

/**
* @brief Управляем потоком записи в файл
*/
class Recorder: public QObject, public IRecorder
{
    Q_OBJECT
public:
  explicit Recorder(QObject *parent, ICamera *camera, const TRecorderSettings &settings);
  ~Recorder();

  bool startRec() override;
  bool stopRec() override;

  bool init();

  bool loadFilesList();
  bool saveFilesList();

  quint32 id() const override;

  QMap<quint64, TRecFileInfo> filesInfo() const override;
  TRecFileInfo fileInfo() const override;

  bool checkNeedNewFile() const override;
  bool makeNewRecFile() override;
  bool makeNewRecFileIfNeed() override;

signals:
  void completed();

public slots:

private slots:
  void onCameraError();
  void onRecorderThreadError();

private:
  ICamera *_camera =nullptr;
  TRecorderSettings _settings;
  QTimer *_makeNewRecFileIfNeedTimer =nullptr;

  TRecFileInfo *_curFileInfo =nullptr;

  QFile _filesListFile;
  using TRecFilesList =QMap<quint64, TRecFileInfo>;
  TRecFilesList _filesList;

  QThread *_thread =nullptr;
  RecorderThread *_recorderThread =nullptr;

  void setRecorderThreadNewRecFileInfo(TRecFileInfo *recFileInfo);
};

#endif // RIPPER_H
