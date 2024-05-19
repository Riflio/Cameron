#ifndef IQSETTINGS_H
#define IQSETTINGS_H

#include <QObject>
#include "interfaces/isettings.h"

/**
* @brief The IQSettings class
*/
class IQSettings: public QObject, public ISettings
{
  Q_OBJECT
public:
  explicit IQSettings(QObject *parent): QObject(parent) {}

signals:
  void loadSettings(QString name, const QXmlStreamReader *xml) override final;
  void loaded() override final;

};


#endif // IQSETTINGS_H
