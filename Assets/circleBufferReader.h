#ifndef CIRCLEBUFFERREADER_H
#define CIRCLEBUFFERREADER_H

#include <QMutex>
#include "Interfaces/ICircleBufferReader.h"

/**
* @brief Читаем из кругового буфера
*/
template<class BufType>
class CircleBufferReader: public ICircleBufferReader<BufType>
{
public:
    CircleBufferReader(): _buffer(nullptr), _prevGetOffset(0), _getOffset(0) { }
    virtual ~CircleBufferReader() {}

    CircleBufferReader(IBuffer<BufType> * buffer): _buffer(buffer), _prevGetOffset(0), _getOffset(0) {  }

    void setBuffer(IBuffer<BufType> * buffer) override
    {
        _buffer = buffer;
        _prevGetOffset = 0;
        _getOffset = 0;
    }


    /**
    * @brief Отдаём текущее смещение из буфера записи
    * @param n - если нужно ещё сместить относительно текущего
    * @return если смещение нельзя трогать, то -1
    */
    int curOffset(int n) const override
    {
        if ( _buffer==nullptr ) { return -1; }

        TBufferInfo inf = _buffer->info();

        int toOffset = (inf.cur-1<0)? inf.size-1 : inf.cur-1;

        if ( n<0 ) { //-- Хотят начать с текущей позиции, за вычетом указанного значения
            int putOffset = inf.cur;
            int buffSize = inf.size;
            toOffset = (putOffset>qAbs(n))? putOffset+n-1 : putOffset+n+buffSize-1;
        }

        if ( (toOffset>=inf.fullnes) || (toOffset<0) || (toOffset==inf.cur) ) { return -1; }

        return toOffset;

    }

    /**
    * @brief Начинаем с указанной позиции
    * @param idx
    * @return
    */
    bool startFrom(int n) override
    {
        int toOffset = curOffset(n);

        if ( toOffset==-1 ) { return false; }

        _getOffset = toOffset;
        _prevGetOffset = toOffset;

        return true;
    }

    /**
    * @brief Отдаём следующий элемент
    * @return
    */
    const BufType * get() override
    {
        if ( _buffer==nullptr ) { return nullptr; }

        TBufferInfo inf = _buffer->info();
        int putOffset = inf.cur;
        int bufSize = inf.size;
        int fullnes = inf.fullnes;

        if ( (_prevGetOffset<=putOffset && _getOffset>=putOffset) || (_prevGetOffset>=_getOffset && _getOffset>=putOffset) ) { //-- Хотим вперёд текущего круга - не даём
            return nullptr;
        }

        if ( _getOffset>=fullnes ) { //-- У нас пока что нет стольки элементов
            return nullptr;
        }

        _prevGetOffset = _getOffset;

        if ( _getOffset+1>=bufSize ) { //-- Сделали круг
            _getOffset = 0;
        } else {
            _getOffset++;
        }

        return _buffer->at(_prevGetOffset);
    }

    const BufType * at(int idx) override
    {
        if ( idx<0 ) return nullptr;
        return _buffer->at(idx);
    }

    TBufferInfo info() const
    {
        TBufferInfo inf = _buffer->info();
        inf.cur = _getOffset;
        return inf;
    }

private:
    IBuffer<BufType> * _buffer;
    int _prevGetOffset; //-- По предыдущему смещению определяем переход на следующий круг
    int _getOffset; //-- Текущее смещение

};

#endif // CIRCLEBUFFERREADER_H
