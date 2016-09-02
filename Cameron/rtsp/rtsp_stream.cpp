#include "rtsp_stream.h"
#include <QDebug>
namespace NS_RSTP {

RTSP_Stream::RTSP_Stream(QObject * parent):
    RTP(parent), _mutex()
{
    _framesOffset = 0;
}

/**
 * @brief Отдаём фрейм из буфера относительно текущей позиции у того, кому нужно (что бы сразу несколько могли читать один буфер).
 * ну и заодно изменяем эту позицию, что бы каждый раз при запросе фрейма мог продолжить с того места, где остановился
 * ну или его пошлют нахуй, если будет долго думать (ограничитель MAX_FRAMES)
 * @return
 */
QByteArray RTSP_Stream::getFrame(int & offset)
{
    QMutexLocker locker(&_mutex);

    if (_frames.empty() || offset>_framesOffset) return QByteArray(); //-- либо вообще нет фреймов, либо выше конца буфера - нет ничего нового для отдачи

    //-- если буфер не заполнен, то за верхнюю границу принимаем то, на сколько заполнен, что бы не ждать заполнения
    int UP_LIMIT = (_framesOffset<MAX_FRAMES)? _framesOffset : MAX_FRAMES;

    if (offset==-1) { //-- не знают актуальной позиции, хотят начать сначала буфера
        offset = (_framesOffset - UP_LIMIT ) + 1;
    }

    int index =  UP_LIMIT - (_framesOffset - offset)  - 1;

    if (index<0) { //-- буфер уже сделал круг, кто обратился за фреймом слишком долго думал - установим его позицию в начало буфера
        offset =  (_framesOffset - UP_LIMIT ) + 2; //-- +2 потому что в этот раз мы уже отдадим элемент
        index = 0;
    } else  {
        offset++;
    }

    return _frames.at(index);

}


/**
 * @brief Отдаём наще текущее смещение, если нужно, то относительно заданного
 * @param cur заданное смещение
 * @return
 */
int RTSP_Stream::getOffset(int cur)
{
    return _framesOffset - cur;
}

/**
 * @brief Добавляем новый фрейм в буфер
 */
void RTSP_Stream::toNewFrame(QByteArray frame)
{
    QMutexLocker locker(&_mutex);

    if (frame.isEmpty()) return;


    if (_frames.size()>=MAX_FRAMES) {
        _frames.takeFirst();
    }

    _frames.append(frame);

    //qDebug()<<"RTSP stream frames count"<<_frames.size();

   _framesOffset++;

    emit newFrame(frame);
}

}
