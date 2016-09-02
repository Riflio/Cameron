#include "settings.h"
#include <QDebug>

Settings::Settings(QObject *parent) : QObject(parent)
{

}


bool Settings::load(TCams & cams)
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

            if (xml.name() == "cameras")  continue;

            if (xml.name() == "camera") {
                QXmlStreamAttributes attributes = xml.attributes();
                if (! attributes.hasAttribute("id") || ! attributes.hasAttribute("url") ) return false;

                Camera * cam = new Camera(0);
                cam->setSettings(attributes.value("url").toString());
                cams.append(cam);

            }
        }

    }



    return true;
}






