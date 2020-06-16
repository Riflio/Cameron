#ifndef RTSP_H
#define RTSP_H

#include <QObject>
#include <QDebug>

#include <QTcpSocket>
#include <QUrl>
#include <QRegExp>

#include "rtsp_channel.h"
#include "sdp/sdp.h"

namespace NS_RSTP {


class RTSP : public QObject
{
    Q_OBJECT
public:
    explicit RTSP(QObject *parent = 0);
    ~RTSP();

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

    RTSP_Channel * getChannel(int id);
    int channelsCount();

    friend class RTSP_Channel;

signals:
    void connected(); //-- Подключились к камере
    void disconnected(); //-- Отключились
    void errored(); //-- Возникла ошибка канала/потока и т.д.
    void setuped(int channel); //-- Создали канал для передачи
    void played(int channel); //-- Начали передачу
    void toTeardown(int channel); //-- Заканчиваем передачу
    void teardowned(int channel); //-- Закончили передачу
    void alived(int channel); //-- Мы ещё живы и ведём передачу

public slots:
    void cameraConnect(QHostAddress address, int port, QString params);
    void cameraConnect(QString url);

private slots:
    void onSckConnectReadyRead();
    void onSckConnectConnected();
    void onSckConnectDisconnected();
    void onSckConnectError(QAbstractSocket::SocketError);    

private:
    QTcpSocket * _sckConnect; //-- Сокет для общения
    QList<RTSP_Channel*> _channels; //-- Все каналы камеры
    QString _gateway;  //-- Собранная строка подключения
    int _reqID; //-- Каждый запрос к камере должен сопровождаться айдишником, в ответ мы его же и получаем

    //-- Нужно знать, на какой запроc по _reqID пришёл ответ
    class ReqHistory {
        public:
        ReqHistory(METHODS m = NONE, int c = -1): method(m), channel(c) {}
        METHODS method=NONE; //-- Какой был метод
        int channel=-1; //-- К какому каналу относится
    };
    QHash<int, ReqHistory> reqHistories;

    SDP * _sdp; //-- Парсер sdp данных

    typedef  QMap<QString, QString> SendParams;
    int send(METHODS method, SendParams params);

    void options();
    void describe();
    void setup(int channel, int port);
    void play(int channel);
    void alive(int channel);
    void teardown(int channel);    
};
}
#endif // RTSP_H
