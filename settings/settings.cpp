#include "settings.h"
#include <QDebug>

Settings::Settings(QObject *parent) : QObject(parent)
{

}


bool Settings::load(TCams & cams, DB * db)
{

    //_cams = cams;

    qInfo()<<"Load settings";

    _settingsFile = new QFile("cameron.xml");

    if (!_settingsFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal("Settings file not opened");
        return false;
    }

    QXmlStreamReader xml(_settingsFile);

    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if (token == QXmlStreamReader::StartDocument || token == QXmlStreamReader::EndDocument) {
            continue;
        }

        if (token == QXmlStreamReader::StartElement) {

            if (xml.name() == "db") {
                QXmlStreamAttributes attributes = xml.attributes();
                if (! attributes.hasAttribute("host") || ! attributes.hasAttribute("db") ) return false;

                db->setSettings(attributes.value("host").toString(), attributes.value("db").toString(), attributes.value("user").toString(), attributes.value("password").toString());

                continue;
            }

            if (xml.name() == "cameras")  continue;

            if (xml.name() == "camera") {
                QXmlStreamAttributes attributes = xml.attributes();
                if (! attributes.hasAttribute("id") || ! attributes.hasAttribute("url") ) return false;

                Camera * cam = new Camera(0);
                cam->setSettings(attributes.value("url").toString(), attributes.value("id").toInt(), attributes.value("ripSecconds").toInt(), attributes.value("ripSavePath").toString() );
                cams.append(cam);

            }
        }

    }



    return true;
}






