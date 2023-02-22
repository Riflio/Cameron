#ifndef ISDP_H
#define ISDP_H

#include <QByteArray>
#include <QMap>
#include <QHostAddress>

class ISDP
{
public:
  virtual ~ISDP() {}

  virtual bool parse(QByteArray data)=0;
  virtual bool make(QByteArray & data)=0;

  //-- Время начала и ожидания сессии
  struct sTiming {
    int start =0;
    int stop =0;
  };

  //-- Атрибуты базовой инфы/медиа
  class sAttribute {
  public:
    QString name; //-- Название
    QString value; //-- Значение
    QMap<QString, QString> parameters; //-- Параметры атрибута
  };

  //-- Базовая инфа
  class sOrigin {
  public:
    int version; //-- Версия протокола
    QString sessionUser; //-- Юзер сессии
    long int sessionID; //-- Идентификатор сессии
    long int sessionVer; //-- Версия сессии
    QString creatorName; //-- Идентификатор владельца
    QString netType; //-- Сетевой протокол
    QHostAddress host; //-- Адрес хоста
    sTiming timing; //-- Начало и завершение трансляции
    QMap<QString, sAttribute*> attribytes; //-- Аттрибуты. Ключ - название атрибута
  };



  //-- Возможные типы медиа
  enum mediaTypes {
    MT_VIDEO,
    MT_AUDIO
  };

  class sMedia {
  public:
    ~sMedia() { qDeleteAll(attribytes); }
    mediaTypes type; //-- Тип
    int port; //-- Порт
    QString profile; //-- Профиль
    QList<QString> codecs; //-- Поддерживаемые кодеки
    QMap<QString, sAttribute*> attribytes; //-- Аттрибуты. Ключ - название атрибута
  };

};

#endif // ISDP_H
