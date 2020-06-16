#ifndef ITHREAD_H
#define ITHREAD_H

#include <QThread>

class IThread
{
public:
    virtual bool start(QThread::Priority priority = QThread::NormalPriority, bool delOnFinish = false) =0;
    virtual bool stop() =0;
    virtual bool pause() =0;
};

#endif // ITHREAD_H
