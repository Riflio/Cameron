#ifndef RTSP_CLIENT_H
#define RTSP_CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QUrl>
#include "Camera/cameras.h"
#include "Server/server_client_streamer.h"
#include "server_client_info.h"

/**
* @brief Подключившийся клиент
* обрабатываем запросы,
* отдаём инфу обо всех камерах
* и т.д.
*/
class Server;
class Server_Client : public Server_Client_Info
{
  Q_OBJECT
public:
  explicit Server_Client(QObject *parent =nullptr, QTcpSocket * socket =nullptr, Server * server =nullptr);
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
  QTcpSocket * _socket;
  Server * _server;
  int _id;

  void answerAlive(int cseq, qint32 sessionId);
  void answerOPTIONS(int cseq);
  void answerDESCRIBE(int cseq, int trackId);
  void answerPLAY(int cseq, qint32 sessionId);
  void answerTEARDOWN(int cseq, qint32 sessionId);
  void answerSETUP(int cseq, int videoPort, int audioPort, qint32 sessionId);
  void answerGETPARAMETER(int cseq, qint32 sessionId);
  void answer(int statusCode, int cseq =0, QByteArray data ="", qint32 sessionId =-1, bool lastRN =true);

  QMap<int, Server_Client_Streamer*> _streamers; //-- <cameraId, Server_Client_Streamer*> Стримеры, которые запустил этот клиент

  void aliveTimeOver();
  QTimer * _aliveTimeOverTimer;

  QString _requestData;

  typedef struct SessionInfo {
    int trackId =-1;
  } TSessionInfo;

  static QMap<qint32, TSessionInfo> _sessions;
  static qint32 generateSessionId();

  inline bool checkSessionExists(qint32 sessionId);

};

#endif // RTSP_CLIENT_H
