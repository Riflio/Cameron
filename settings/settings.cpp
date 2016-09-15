#include "settings.h"
#include <QDebug>

Settings::Settings(QObject *parent) : QObject(parent)
{

}


bool Settings::load(Cameras * cameras, Server * server)
{

    qInfo()<<"Load settings";

    _settingsFile = new QFile("cameron.xml");

    if (!_settingsFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal("Settings file not opened");
        return false;
    }

    QXmlStreamReader xml(_settingsFile);

    if (xml.hasError()) return false;

    while ( !xml.atEnd()  ) {

        if (xml.hasError()) return false;

        QXmlStreamReader::TokenType token = xml.readNext();

        if (token == QXmlStreamReader::StartDocument || token == QXmlStreamReader::EndDocument) {
            continue;
        }

        if (token == QXmlStreamReader::StartElement) {

            if (xml.name() == "cameras")  continue;

            if (xml.name() == "camera") {
                QXmlStreamAttributes attributes = xml.attributes();
                if (! attributes.hasAttribute("id") || ! attributes.hasAttribute("url") ) return false;

                cameras->newCam(attributes.value("id").toInt())->setSettings( attributes.value("url").toString(), attributes.value("id").toInt() );

            }

            if (xml.name() == "server") {
                QXmlStreamAttributes attributes = xml.attributes();
                server->setSettings( attributes.value("host").toString(), attributes.value("port").toInt() );
            }
        }

    }



    return true;
}






