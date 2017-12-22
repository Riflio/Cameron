#ifndef WTHREAD_H
#define WTHREAD_H

#include <QObject>
#include <QThread>
#include <QApplication>
#include <QDebug>
#include "../Interfaces/ithread.h"

class WThread: public QObject, public IThread
{
    Q_OBJECT
public:

    WThread(QObject * parent): QObject(0)
    {
        _status = 0;
        _thread = new QThread(0); //-- поток не должен иметь родителя, что бы он не снёс его ненароком, пока выполняется
        connect(_thread, &QThread::started, this, &WThread::process, Qt::DirectConnection ); //-- при запуске потока сразу начинаем работу
        connect(_thread, &QThread::finished, this, &WThread::del, Qt::DirectConnection  ); //-- как только поток остановился, удаляемся
        if (parent!=NULL) connect(parent, &QObject::destroyed, this, &WThread::del, Qt::DirectConnection ); //-- Как только удаляется родитель, удаляемся и мы

        moveToThread(_thread);        
    }


    enum WThreadStatus {
        WT_STARTED=4,
        WT_RUNNING=8,
        WT_PAUSED=16,
        WT_STOPED=32,
        WT_FINISHED=64,
        WT_DESTROYED=128
    };


    virtual void process()=0;

    virtual int status()
    {
        return _status;
    }

    virtual bool start()
    {
        if ( !(_status & WT_STARTED) &&  !_thread->isRunning()) {
            _thread->start();
            _status |= WT_STARTED;
        }

        _status &= ~WT_PAUSED;
        _status |= WT_RUNNING;

        emit statusChanged();

        return true;
    }



    virtual bool stop()
    {
        qDebug()<<"";
        if ( (_status&WT_STOPED) && !(_status&WT_RUNNING)) return false; //-- Если уже и так останавливаемся или не выполняется

        _status &= ~WT_RUNNING;
        _status |= WT_STOPED;

        emit statusChanged();

        if ( !(_status&WT_STARTED) ) { //-- Если не был запущен, значит сразу можно удалять
            del();
        }

        return true;
    }

    virtual bool pause()
    {
        if ( !(_status&WT_RUNNING) ) return false;

        _status &= ~WT_RUNNING;
        _status |= WT_PAUSED;

        emit statusChanged();

        return true;

    }

protected slots:
    void del() {
        qDebug()<<"";
        if (_status&WT_RUNNING) { //-- Если мы запущены, сначала останавливаемся
            stop();
            return;
        }

        _status |= WT_DESTROYED;
        emit statusChanged();

        QApplication::processEvents();

        _thread->deleteLater();
        deleteLater();
    }

    void finish()
    {
        qDebug()<<"";
        _status &= ~WT_RUNNING;
        _status |= WT_FINISHED;
        emit statusChanged();
        emit finished();
        _thread->quit();
    }

signals:
    void finished();
    void statusChanged();

protected:
    QThread * _thread;
    int _status;

};

#endif // WTHREAD_H
