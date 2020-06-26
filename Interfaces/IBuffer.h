#ifndef IBUFFER_H
#define IBUFFER_H

/**
* @brief Базовый интерфейс буфера
*/

struct TBufferInfo {
    int cur;
    int fullnes;
    int size;
};

template<class BufType>
class IBuffer
{
public:
    virtual TBufferInfo info() const =0;
    virtual const BufType * at(int idx) =0;
};

#endif // IBUFFER_H
