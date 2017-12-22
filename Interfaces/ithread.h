#ifndef ITHREAD_H
#define ITHREAD_H

class IThread
{
public:
    virtual bool start()=0;
    virtual bool stop()=0;
    virtual bool pause()=0;
    virtual int status()=0;
};

#endif // ITHREAD_H
