#ifndef MULTIACCESSBUFFER_H
#define MULTIACCESSBUFFER_H

#include <QList>
#include <QByteArray>
#include <QMutexLocker>
#include <QDebug>
/**
* @brief The MultiAccessBuffer class
*/

template <class BufType>  class MultiAccessBuffer
{
public:
    MultiAccessBuffer(long long int max): _max(max), _offset(0), _mutex() {  }

    bool put(const BufType & data)
    {
        QMutexLocker locker(&_mutex);

        if (_buffer.size()>=_max) {
            _buffer.takeFirst();
        }

        _buffer.append(data);

        _offset++;

        return true;
    }

    bool get(long long int & offset, BufType & data) {
        QMutexLocker locker(&_mutex);

        if (_buffer.empty() || offset>_offset) return false; //-- либо вообще нет данных, либо выше конца буфера - нет ничего нового для отдачи

        //-- если буфер не заполнен, то за верхнюю границу принимаем то, на сколько заполнен, что бы не ждать заполнения
        int limit = (_offset<_max)? _offset : _max;

        if (offset==-1) { //-- не знают актуальной позиции, хотят начать сначала буфера
            offset = (_offset - limit ) + 1;
        }

        int index =  limit - (_offset - offset)  - 1;

        if (index<0) { //-- буфер уже сделал круг, кто обратился за данными, тот слишком долго думал - установим его позицию в начало буфера
            offset =  (_offset - limit ) + 2; //-- +2 потому что в этот раз мы уже отдадим элемент
            index = 0;
        } else  {
            offset++;
        }


        data = _buffer.at(index);

        return true;
    }

    long long offset(long long int cur=0)  { return _offset - cur; }



private:
    QList<BufType> _buffer;
    long long int _offset;
    long long int _max; //-- максимальный размер буфера

    QMutex _mutex;
};

#endif // MULTIACCESSBUFFER_H
