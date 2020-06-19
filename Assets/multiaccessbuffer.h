#ifndef MULTIACCESSBUFFER_H
#define MULTIACCESSBUFFER_H

#include <QVector>
#include <QMutexLocker>

/**
* @brief The MultiAccessBuffer class
* Потоко-безопасный круговой буфер размером (max) с доступом нескольких желающих с того места+1, где они в прошлый раз брали данные (get) не зависимо от остальных
*/

template <class BufType>  class MultiAccessBuffer
{
public:
    MultiAccessBuffer(int max=10): _buffSize(0), _max(max), _offset(0), _mutex() {  }

    bool put(const BufType & data)
    {
        QMutexLocker locker(&_mutex);

        if ( _buffSize<_max ) { //-- Заполняем, пока можем
            _buffer.append(data);
            _buffSize++;
            _offset++;
            return true;
        }

        if ( _offset>=_max ) { //-- Пошли на круг
            _offset = 0;
        }

        _buffer[_offset] = data;
        _offset++;

        return true;
    }

    const BufType * get(int & offset) const
    {
        QMutexLocker locker(&_mutex);

        if ( offset>=_max) { offset = 0; } //-- Сделали круг
        if ( _buffer.empty() || (offset>=_offset) ) { return nullptr; } //-- Либо вообще нет данных, либо выше конца буфера - нет ничего нового для отдачи

        if ( offset<0 ) { //-- Хотят начать с текущей позиции, за вычетом указанного значения
            int toOffset = (_offset>qAbs(offset))? _offset+offset-1 : _offset+offset+_max-1;
            if ( toOffset>=_buffSize ) { return nullptr; } //-- Мы ещё столько не накопили
            offset = toOffset;
        }

        int idx = offset;

        offset += 1; //-- Что бы в следующий раз сразу следующий элемент получил

        return &_buffer.at(idx);
    }

    int cur() const
    {
        return _offset-1;
    }

    int size() const
    {
        return _buffSize;
    }

private:
    QVector<BufType> _buffer;
    int _buffSize; //-- Текущая заполненность буфера
    int _max; //-- Максимальный размер буфера
    int _offset; //-- Индекс, куда добавлять будем данные
    mutable QMutex _mutex;

};

#endif // MULTIACCESSBUFFER_H
