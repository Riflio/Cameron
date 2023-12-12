#ifndef RTSP_H
#define RTSP_H

#include <QObject>
#include <QDebug>

#include <QTcpSocket>
#include <QAuthenticator>
#include <QtNetwork/private/qauthenticator_p.h>
#include <QUrl>

#include "sdp/sdp.h"

namespace NS_RSTP {

/**
* @brief Управляем каналами вещания камеры
*/
class RTSP_Channel;
class RTSP : public QObject
{
  Q_OBJECT
public:
  explicit RTSP(QObject *parent = 0);
  ~RTSP();

  const QByteArray RTSPVERSION ="RTSP/1.0";

  enum METHODS{
    NONE,
    OPTIONS,
    DESCRIBE,
    SETUP,
    PLAY,
    GET_PARAMETER,
    TEARDOWN,
    ALIVE
  };

  RTSP_Channel *getChannel(int id);
  int channelsCount();

  friend class RTSP_Channel;

signals:
  void connected(); //-- Подключились к камере
  void disconnected(); //-- Отключились
  void errored(); //-- Возникла ошибка канала/потока и т.д.


public slots:
  void cameraConnect(QString url);
  void cameraDisconnect();

private slots:
  void onSckConnectReadyRead();
  void onSckConnectConnected();
  void onSckConnectDisconnected();
  void onSckConnectError(QAbstractSocket::SocketError);

protected:
  typedef QPair<QByteArray, QByteArray> THeader;
  typedef QList<QPair<QByteArray, QByteArray>> THeaders;
  int send(METHODS method, const THeaders &headers, uint16_t channel);

private:
  QTcpSocket *_sckConnect =nullptr; //-- Сокет для общения
  QAuthenticatorPrivate _authenticator; //-- Для обработки авторизации на камере
  QList<RTSP_Channel*> _channels; //-- Все каналы камеры
  QUrl _gateway;  //-- Собранная строка подключения
  int _reqID; //-- Каждый запрос к камере должен сопровождаться айдишником, в ответ мы его же и получаем

  //-- Нужно знать, на какой запроc по _reqID пришёл ответ
  class ReqHistory {
  public:
    ReqHistory(METHODS m = NONE, int c = -1): method(m), channel(c) {}
    METHODS method=NONE; //-- Какой был метод
    int channel=-1; //-- К какому каналу относится
  };
  QHash<int, ReqHistory> reqHistories;

  SDP *_sdp =nullptr; //-- Парсер sdp данных

  void options();
  void describe();

};
}
#endif // RTSP_H
