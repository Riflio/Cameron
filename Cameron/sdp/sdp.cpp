#include "sdp.h"
#include <QDebug>

SDP::SDP(QObject *parent) : QObject(parent)
{

}


bool SDP::parse(QByteArray data)
{
    QString dataText = data;

    QList<QString> dataList = dataText.split("\r\n");

    QRegExp rxc("^([a-z])=(.*)", Qt::CaseInsensitive, QRegExp::RegExp2); //-- парсим построчно на команду и её выражение
    QRegExp rxa("([^:]+):([^\\s]*)(.*)", Qt::CaseInsensitive, QRegExp::RegExp2); //-- парсим выражение на отдельные параметр - значение - дополнительные параметры
    QRegExp rxap("([^=\\s]+)=([^;]+)(;|$)", Qt::CaseInsensitive, QRegExp::RegExp2); //-- парсим выражение дополнительных параметров на отдельные параметр - значение


    int mediaIndex=-1; //-- что бы знать, к какой медиа записи относяться команды

    //TODO: доделать остальные команды
    foreach (QString dataStr, dataList) {

        if (rxc.indexIn(dataStr)>-1) {

            QString command = rxc.cap(1);
            QString term = rxc.cap(2);

            if (command=="v") {
                origin.version = term.toInt();
            }

            if (command=="s") {
                origin.creatorName = term;
            }

            if (command=="m") {

                mediaIndex++;

                QList<QString> params = term.split(" "); //-- разделяем выражение на отдельные значения по пробелу

                sMedia * media = new sMedia();
                media->type = (params.takeAt(0)=="video")? MT_VIDEO : MT_AUDIO;
                media->port = params.takeAt(0).toInt();
                media->profile = params.takeAt(0);
                media->codecs = params; //-- всё оставшееся

                medias.append(media);
            }

            if (command=="a") {

                sAttribute * attribute = new sAttribute();

                if (rxa.indexIn(term)>-1) { //-- если есть раздельные параметр-значение

                    attribute->name = rxa.cap(1);
                    attribute->value = rxa.cap(2);
                    QString extraParams = rxa.cap(3);

                    if (extraParams.length()>0) {
                        extraParams+=";";
                        int pos=rxap.indexIn(extraParams, 0); //-- ищем первое совпадение


                        if (pos==-1) {//-- первого нет, значит только одно значение
                            attribute->parameters.insert(extraParams, extraParams);
                        } else { //-- первое есть, парсим его и ищем дальше
                            do {
                                attribute->parameters.insert(rxap.cap(1), rxap.cap(2));
                                pos=rxap.indexIn(extraParams, pos+rxap.matchedLength());
                            } while(pos!=-1);
                        }
                    }

                } else { //-- выражение состоит лишь из одного названия параметра
                    attribute->name = term;
                }

                //-- решаем что делать с атрибутом
                if (mediaIndex!=-1) { //-- если есть медиа записи, то значит относиться к ним
                    medias.at(mediaIndex)->attribytes.insertMulti(attribute->name, attribute);
                }

            }

        }
    }

    return true;

}

SDP::~SDP()
{
    //TODO: доделать бы освобождение памяти :P
}







