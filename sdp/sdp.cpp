#include "sdp.h"
#include <QDebug>

SDP::SDP(QObject *parent) : QObject(parent)
{

}

/**
* @brief Парсим SDP данные
* @param data
* @return
*/
bool SDP::parse(QByteArray data)
{
    qInfo()<<"SDP Parse"<<data;

    qDeleteAll(medias);
    medias.clear();
    origin = sOrigin();

    QString dataText = data;

    QList<QString> dataList = dataText.split("\r\n");

    QRegExp rxc("^([a-z])=(.*)", Qt::CaseInsensitive, QRegExp::RegExp2); //-- Парсим построчно на команду и её выражение
    QRegExp rxa("([^:]+):([^\\s]*)(.*)", Qt::CaseInsensitive, QRegExp::RegExp2); //-- Парсим выражение на отдельные параметр - значение - дополнительные параметры
    QRegExp rxap("([^=\\s]+)=([^;]+)(;|$)", Qt::CaseInsensitive, QRegExp::RegExp2); //-- Парсим выражение дополнительных параметров на отдельные параметр - значение


    int mediaIndex=-1; //-- Что бы знать, к какой медиа записи относяться команды

    //TODO: Доделать остальные команды
    foreach (QString dataStr, dataList) {

        if ( rxc.indexIn(dataStr)>-1 ) {

            QString command = rxc.cap(1);
            QString term = rxc.cap(2);

            if ( command=="v" ) {
                origin.version = term.toInt();
            }

            if ( command=="s" ) {
                origin.creatorName = term;
            }

            if ( command=="m" ) {

                mediaIndex++;

                QList<QString> params = term.split(" "); //-- Разделяем выражение на отдельные значения по пробелу

                sMedia * media = new sMedia();
                media->type = (params.takeAt(0)=="video")? MT_VIDEO : MT_AUDIO;
                media->port = params.takeAt(0).toInt();
                media->profile = params.takeAt(0);
                media->codecs = params; //-- Всё оставшееся

                medias.append(media);
            }

            if ( command=="a" ) {

                sAttribute * attribute = new sAttribute();

                if ( rxa.indexIn(term)>-1 ) { //-- Если есть раздельные параметр-значение

                    attribute->name = rxa.cap(1);
                    attribute->value = rxa.cap(2);
                    QString extraParams = rxa.cap(3);

                    if (extraParams.length()>0) {

                        int pos=rxap.indexIn(extraParams+";", 0); //-- Ищем первое совпадение

                        if ( pos==-1 ) {//-- Первого нет, значит только одно значение
                            attribute->parameters.insert(extraParams, extraParams);
                        } else { //-- Первое есть, парсим его и ищем дальше
                            do {
                                attribute->parameters.insert(rxap.cap(1), rxap.cap(2));
                                pos=rxap.indexIn(extraParams+";", pos+rxap.matchedLength());
                            } while (pos!=-1);
                        }
                    }

                } else { //-- Выражение состоит лишь из одного названия параметра
                    attribute->name = term;
                }

                //-- Решаем что делать с атрибутом
                if ( mediaIndex!=-1 ) { //-- Если есть медиа записи, то значит относиться к ним
                    medias.at(mediaIndex)->attribytes.insertMulti(attribute->name, attribute);
                }

            }

        }
    }

    return true;

}

/**
 * @brief Формируем SDP данные
 * @param data
 * @return
 */
bool SDP::make(QByteArray & data)
{
    QString rn= "\r\n";

    auto makeAttributes = [=](const QMap<QString, sAttribute*> & attribytes)->QString { //-- Формирует атрибуты базовые, медиазаписи и т.д.
        QString at;
        QMap<QString, SDP::sAttribute*>::const_iterator j;
        for (j=attribytes.begin(); j!= attribytes.end(); ++j) {
            sAttribute * a = j.value();

            //-- Пошли по параметрам атрибута медиа записи
            QStringList p;
            QMap<QString, QString>::iterator l;
            for (l=a->parameters.begin(); l!= a->parameters.end(); ++l) {
                if ( l.key()==l.value() ) { //-- Когда название равно значению нужно писать лишь значение
                    p.append(l.value());
                } else {
                    p.append(QString("%1=%2").arg(l.key()).arg(l.value()));
                }
            }

            at.append(QString("a=%1%2%3").arg(a->name).arg((a->value!="")?":":"").arg(a->value));

            if (p.count()>0) { //-- Если есть параметры, нужно добавить через пробел (иначе пробела лишнего не надо)
                at.append(QString(" %1").arg(p.join("; ")));
            }

            at.append(rn);
        }

        return at;

    };

    QString dt;    
    QString host = QString("%1 %2 %3").arg(origin.netType).arg((origin.host.protocol()==QAbstractSocket::IPv4Protocol)?"IP4":"IP6").arg(origin.host.toString());

    //-- Пишем базовое
    dt.append(QString("v=%1").arg(origin.version)).append(rn);
    dt.append(QString("s=%1").arg(origin.creatorName)).append(rn);
    dt.append(QString("c=%1").arg(host)).append(rn);
    dt.append(QString("t=%1 %2").arg(origin.timing.start).arg(origin.timing.stop)).append(rn);
    dt.append(QString("o=%1 %2 %3 %4").arg((origin.sessionUser=="")? "-" : origin.sessionUser).arg(origin.sessionID).arg(origin.sessionVer).arg(host)).append(rn);
    dt.append(makeAttributes(origin.attribytes));

    //-- Пошли по медиа-записям
    QList<sMedia*>::iterator i;
    for (i=medias.begin(); i!= medias.end(); ++i) {

        sMedia* m = *i;

        dt.append(QString("m=%1 %2 %3 %4").arg( (m->type==MT_VIDEO)?"video":(m->type==MT_AUDIO)?"audio":"").arg(m->port).arg(m->profile).arg(m->codecs.join(" ")) ).append(rn);

        //-- Формируем атрибуты
        dt.append( makeAttributes( m->attribytes ) );

    }    

    //-- Да пошло оно всё, заебало
    data = QByteArray(dt.toUtf8());
    return true;
}

SDP::~SDP()
{
    qDeleteAll(medias);
}

