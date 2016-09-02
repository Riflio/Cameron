#ifndef SDP_H
#define SDP_H

#include <QObject>
#include <QMap>
#include <QMultiMap>


/**
 * @brief Парсим SDP формат
 */
class SDP : public QObject
{
    Q_OBJECT
public:
    explicit SDP(QObject *parent = 0);
    ~SDP();

    bool parse(QByteArray data);

    //-- время начала и ожидания сессии
    struct sTiming {
        int start;
        int stop;
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
        QString sessionName; //-- название сессии
        QString creatorName; //-- идентификатор владельца
        QString netType; //-- сетевой протокол
        int ipVersion; //-- версия сетевого протокола
        QString adress; //-- адрес сетевого подключения
        sTiming timing;
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
        QMap<QString, SDP::sAttribute*> attribytes; //-- аттрибуты. Ключ - название атрибута
    };


    sOrigin origin;
    QList<sMedia*> medias;

signals:

public slots:

private:
    QByteArray _data;
};

#endif // SDP_H
