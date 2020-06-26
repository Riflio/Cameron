#ifndef MULTIACCESSBUFFER_H
#define MULTIACCESSBUFFER_H

#include <QVector>
#include "Interfaces/ICircleBufferWriter.h"

/**
* @brief Пишем в круговой буфер
*
*/
template <class BufType>
class CircleBufferWriter: public ICircleBufferWriter<BufType>
{
public:
    CircleBufferWriter(int bufSize=10): _buffSize(bufSize), _fullnes(0), _putOffset(0), _info({0,0,bufSize})
    {
        _buffer.resize(bufSize);
    }

    virtual ~CircleBufferWriter() {}

    bool put(const BufType &itm) override
    {
        return put<BufType>(itm);
    }

    template<class T>
    bool put(const T &itm)
    {
        _buffer[_putOffset] = itm;

        if ( _putOffset+1>=_buffSize ) { //-- Пошли на круг
            _putOffset = 0;
        } else {
            _putOffset++;

            if ( _fullnes<_buffSize ) {
                _fullnes++;
            }
        }

        _info = {_putOffset, _fullnes, _buffSize};

        return true;
    }

    const BufType * at(int idx) override
    {
        return &_buffer.at(idx);
    }

    TBufferInfo info() const
    {
        return _info;
    }

private:
    QVector<BufType> _buffer;
    int _buffSize; //-- Размер буфера
    int _fullnes; //-- Заполненность
    int _putOffset; //-- Текущее смещение, куда будем вставлять
    TBufferInfo _info;

};

#endif // MULTIACCESSBUFFER_H
