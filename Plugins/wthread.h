#ifndef WTHREAD_H
#define WTHREAD_H

#include <QObject>
#include <QThread>
#include <QEventLoop>
#include <QCoreApplication>
#include <QTimer>
#include <QVariant>

#include "PluginEventsWrapper.h"
#include "Interfaces/ithread.h"

#include "defines.h"


/**
* Базовый класс параллельного потока
* По завершению process() нужно вызывать сигнал finished()
*/
class PluginsManager;
class WThread: public QObject, public IThread
{
    Q_OBJECT
public:

    enum WThreadStatus {
        WT_STARTED=2,
        WT_RUNNING=4,
        WT_PAUSED=16
    };

    WThread(QObject * parent, QString name):
        QObject(parent), _thread(nullptr), _eventLoop(nullptr),
        _loopTimer(nullptr), m_id(0), m_name(name), m_status(0), m_progress(0)
    {
        emit Events->doAction("getWThreadClass", QVariantList()<<Events->ARG(this));
        emit nameChanged(name);

        init(parent);
    }

    virtual ~WThread() { }

    virtual void loop()=0;

    W_FUNCT(WThread, virtual, bool, start,, W_ARG(QThread::Priority, priority, =QThread::NormalPriority), W_ARG(bool, delOnFinish, =false))
    W_FUNCT(WThread, virtual, bool, stop,)
    W_FUNCT(WThread, virtual, bool, pause,)

    W_FUNCT(WThread, virtual, void, onFinished,)
    W_FUNCT(WThread, virtual, bool, onStarted,)
    W_FUNCT(WThread, virtual, bool, onAfterStarted,)

    W_FUNCT(WThread,,int, status, const)
    W_FUNCT(WThread,,QString, msg, const)
    W_FUNCT(WThread,,QString, name, const)
    W_FUNCT(WThread,,void, setProgress,, W_ARG(double,progress))
    W_FUNCT(WThread,,void, setMsg,, W_ARG(QString,msg))
    W_FUNCT(WThread,,double, progress, const)
    W_FUNCT(WThread,,void, setID,, W_ARG(int,id))
    W_FUNCT(WThread,,int,id, const)


public slots:

signals:
    void started();
    void finished();
    void progressChanged(double progress);
    void idChanged(int id);
    void statusChanged(int status); //-- С сигнале специально не сообщаем статус, что бы тянулись всегда за status() что бы получить актуальный, т.к. пока одно дойдёт уже может смениться
    void msgChanged(QString msg);
    void nameChanged(QString name);


protected:
    friend PluginsManager;

    QThread  * _thread;
    QEventLoop * _eventLoop;
    QTimer * _loopTimer;

    int m_id;
    QString m_name;
    int m_status;
    double m_progress;
    QString m_msg;


    W_FUNCT(WThread,,void, init,,W_ARG(QObject*,parent))
    W_FUNCT(WThread,,void, process,)

};

#endif // WTHREAD_H
