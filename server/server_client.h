#ifndef RTSP_CLIENT_H
#define RTSP_CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QUrl>
#include "camera/cameras.h"
#include "server/server_client_streamer.h"
#include "server_client_info.h"

/**
* @brief Подключившийся клиент
* обрабатываем запросы,
* отдаём инфу обо всех камерах
* и т.д.
*/
class Server;
class Server_Client: public Server_Client_Info
{
  Q_OBJECT
public:
  explicit Server_Client(QObject *parent =nullptr, QTcpSocket * socket =nullptr, Server * server =nullptr, int clientID =0);
  ~Server_Client();

  bool loginUser(QString uName, QString uPass);

signals:
  void notAlive(int id); //-- Мы, кажется, не в сети

public slots:
  void request();
  void streamFinished(int streamID);
  int clientID();

private slots:
  void onCameraErrored();

private:
  QTcpSocket *_socket =nullptr;
  Server *_server =nullptr;
  int _id;

  void answerAlive(int cseq, uint32_t sessionId);
  void answerOPTIONS(int cseq);
  void answerDESCRIBE(int cseq, int trackId);
  void answerPLAY(int cseq, uint32_t sessionId);
  void answerTEARDOWN(int cseq, uint32_t sessionId);
  void answerSETUP(int cseq, int videoPort, int audioPort, uint32_t sessionId);
  void answerGETPARAMETER(int cseq, quint32 sessionId);
  void answer(int statusCode, int cseq =0, QByteArray data ="", uint32_t sessionId =0, bool lastRN =true);

  struct TServerClientStreamerThread {
    QThread *thread =nullptr;
    Server_Client_Streamer *streamer =nullptr;
  };

  QMap<int, TServerClientStreamerThread> _streamers; //-- <cameraId, TServerClientStreamerThread> Стримеры, которые запустил этот клиент

  void aliveTimeOver();
  QTimer *_aliveTimeOverTimer;

  QString _requestData;

  typedef struct SessionInfo {
    int trackId =-1;
    uint32_t blockSize =1400;
  } TSessionInfo;

  static QMap<quint32, TSessionInfo> _sessions;
  static quint32 generateSessionId();

  inline bool checkSessionExists(quint32 sessionId);

};

#endif // RTSP_CLIENT_H
