#ifndef RECORDERTHREAD_H
#define RECORDERTHREAD_H

#include <QObject>
#include <QFile>
#include "interfaces/icamera.h"
#include "interfaces/recfileinfo.h"

/**
* @brief Получаем кадры и пишем в файл
*/
class RecorderThread: public QObject
{
  Q_OBJECT
public:
  explicit RecorderThread();
  ~RecorderThread();

signals:
  void errored();

public slots:
  bool setCamera(ICamera *camera);
  bool setRecFileInfo(TRecFileInfo *recFileInfo);

  void onStarted();
  void onNewFrameAvaliable(QSharedPointer<IRTPPacket> packet);

private slots:
  void processNewFrame(QSharedPointer<IRTPPacket> packet);

protected:
  bool writeToOutFile(const QByteArray &data);
  bool writeToOutFile(uint8_t data);
  bool writeToOutFile(uint8_t *data, size_t dataSize);

private:
  bool _busy =false;
  ICamera *_camera =nullptr;
  TRecFileInfo *_recFileInfo =nullptr;
  QFile *_outFile =nullptr;
  QByteArray _frame;
  bool _hasFirstIDRFrame =false;
};

#endif // RECORDERTHREAD_H
