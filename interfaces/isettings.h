#ifndef ISETTINGS_H
#define ISETTINGS_H

#include <QObject>
#include <QXmlStreamReader>

class ISettings
{
public:
  virtual ~ISettings() {}

signals:
  virtual void loadSettings(QString name, const QXmlStreamReader *xml) =0;
  virtual void loaded() =0;

};

#endif // ISETTINGS_H
