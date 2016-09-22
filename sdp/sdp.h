#ifndef SDP_H
#define SDP_H

#include <QObject>
#include <QMap>
#include <QMultiMap>
#include <QHostAddress>

/**
 * @brief Парсим/формируем SDP формат
 */
class SDP : public QObject
{
    Q_OBJECT
public:
    explicit SDP(QObject *parent = 0);
    ~SDP();

    bool parse(QByteArray data);
    bool make(QByteArray & data);

    //-- время начала и ожидания сессии
    struct sTiming {
        int start =0;
        int stop =0;
    };

    //-- Атрибуты базовой инфы/медиа
    class sAttribute {
        public:
        QString name; //-- название
        QString value; //-- значение
        QMap<QString, QString> parameters; //-- параметры атрибута
    };

    //-- базовая инфа
    class sOrigin {
    public:
        int version; //-- версия протокола
        QString sessionUser; //-- юзер сессии
        long int sessionID; //-- идентификатор сессии
        long int sessionVer; //-- версия сессии
        QString creatorName; //-- идентификатор владельца
        QString netType; //-- сетевой протокол
        QHostAddress host; //-- адрес хоста
        sTiming timing; //-- начало и завершение трансляции
        QMap<QString, sAttribute*> attribytes; //-- аттрибуты. Ключ - название атрибута
    };



    //-- Возможные типы медиа
    enum mediaTypes {
        MT_VIDEO,
        MT_AUDIO
    };

    class sMedia {
        public:
        mediaTypes type; //-- тип
        int port; //-- порт
        QString profile; //-- профиль
        QList<QString> codecs; //-- поддерживаемые кодеки
        QMap<QString, sAttribute*> attribytes; //-- аттрибуты. Ключ - название атрибута
    };

    sOrigin origin;
    QList<sMedia*> medias;

signals:

public slots:

private:
    QByteArray _data;
};

#endif // SDP_H
