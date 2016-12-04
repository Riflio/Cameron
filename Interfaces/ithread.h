#ifndef ITHREAD_H
#define ITHREAD_H

class IThread
{
public:
    virtual bool start()=0;
    virtual bool stop()=0;
    virtual bool statys()=0;
    virtual bool onStarted()=0;
};

#endif // ITHREAD_H
