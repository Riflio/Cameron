#ifndef RECORDERTHREAD_H
#define RECORDERTHREAD_H

#include <QObject>
#include <QFile>
#include "interfaces/iqrtspstream.h"
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
  bool setStreamer(NS_RSTP::IRTSPStream *streamer);
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
  NS_RSTP::IRTSPStream *_streamer =nullptr;
  TRecFileInfo *_recFileInfo =nullptr;
  QFile *_outFile =nullptr;
  bool _hasMetaInfo =false;
  QByteArray _frame;
};

#endif // RECORDERTHREAD_H
