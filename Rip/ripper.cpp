#include "ripper.h"

#include <QDebug>

Ripper::Ripper(RTSP_Stream * streamer, QString folder) : QObject(0)
{
    qDebug()<<"NEW RIPPER";
    _toStop = false;
    _framesOffset = -1;
    _streamer = streamer;
    _folder = folder;
    _savePath = QString("%1/rip-%2.h264").arg(_folder).arg(QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss"));
}

void Ripper::process()
{
    _toStop = false;

    //-- создаём новый файл для записи
    QFile outFile(_savePath);
    outFile.open(QIODevice::WriteOnly);

    //-- первым делом нужно получить SPS и PPS и засунуть в файл
    H264_Stream_RTP * rtpStream = dynamic_cast<H264_Stream_RTP *>( _streamer);
    outFile.write( rtpStream->getPS() );

    while(true) {

        if (_toStop) break;

        QByteArray frame = _streamer->getFrame(_framesOffset);

        outFile.write(frame);

    }

    outFile.close();

    qDebug()<<"RIPPER COMPLETE";
    emit complete();
}

void Ripper::stop()
{
    _toStop = true;
}

QString Ripper::savePath()
{
    return _savePath;
}


Ripper::~Ripper()
{    
    qDebug()<<"DELETE RIPPER";
}
