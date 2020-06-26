#ifndef ICIRCLEBUFFERREADER_H
#define ICIRCLEBUFFERREADER_H

#include "IBuffer.h"

template <class BufType>
class ICircleBufferReader: public IBuffer<BufType>
{
public:
    virtual const BufType * get() =0;
    virtual bool startFrom(int idx) =0;
    virtual int curOffset(int n) const =0;
    virtual void setBuffer(IBuffer<BufType> * buffer) =0;
};

#endif // ICIRCLEBUFFERREADER_H
