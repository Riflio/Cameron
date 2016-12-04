#ifndef WTHREAD_H
#define WTHREAD_H

#include <QObject>
#include <QThread>

#include "../Interfaces/ithread.h"

class WThread: public QObject, public IThread
{
    Q_OBJECT
public:

    WThread(QObject * parent): QObject(0)
    {
        _thread = new QThread(0); //-- поток не должен иметь родителя, что бы он не снёс его ненароком, пока выполняется
        connect(_thread, &QThread::started, this, &WThread::process); //-- при запуске потока сразу начинаем работу
        connect(this, &WThread::forceStart, this, &WThread::process); //-- когда нужен принудительный запуск без запуска потока/либо когда уже в потоке, но вылетели из process()
        connect( this, &WThread::finished, _thread, &QThread::quit ); //-- как только вся работа сделана или остановлена, останавливаем поток
        connect(_thread, &QThread::finished, this, &QObject::deleteLater ); //-- как только поток закончен удаляемся
        connect(this, &QObject::destroyed, _thread, &QThread::deleteLater ); //-- Как только удалились, удаляем сам поток
        connect(parent, &QObject::destroyed, this, &WThread::stop); //-- как только родитель удаляется, останавливаем поток
        moveToThread(_thread);

        _started = false;
    }


    virtual void process()=0;

    virtual bool statys()
    {
        return _started;
    }

    virtual bool start()
    {
        if (_started) return true;

        _started = true;

        if (!_thread->isRunning()) {
            _thread->start();
        } else {
            emit forceStart();
        }

        return onStarted();
    }


    virtual bool onStarted()
    {
        return true;
    }



    virtual bool stop()
    {
        if (!_started) {
            this->deleteLater();
        }
        _started = false;

        return true;
    }


signals:
    void finished();
    void forceStart();

protected:
    QThread * _thread;
    bool _started;

};

#endif // WTHREAD_H
