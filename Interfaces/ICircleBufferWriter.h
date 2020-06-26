#ifndef IFRAMEBUFFER_H
#define IFRAMEBUFFER_H

#include "IBuffer.h"

template <class BufType>
class ICircleBufferWriter: public IBuffer<BufType>
{
public:    
    virtual bool put(const BufType &itm) =0;
};

#endif // IFRAMEBUFFER_H
