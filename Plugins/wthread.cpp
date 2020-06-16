#include "wthread.h"
#include <QGuiApplication>

void WThread::_init(QObject * parent)
{
    Q_UNUSED(parent);
}

int WThread::_id() const
{
    return m_id;
}

int WThread::_status() const
{
    return m_status;
}

QString WThread::_name() const
{
    return m_name;
}

double WThread::_progress() const
{
    return m_progress;
}

QString WThread::_msg() const
{
    return m_msg;
}

void WThread::_setProgress(double progress)
{
    if (m_progress == progress) return;

    m_progress = progress;
    emit progressChanged(progress);
}

void WThread::_setMsg(QString msg)
{
    if (m_msg == msg) return;

    m_msg = msg;

    emit msgChanged(m_msg);
}


void WThread::_setID(int id)
{
    m_id = id;
    emit idChanged(m_id);
}


bool WThread::_start(QThread::Priority priority, bool delOnFinish)
{
    if ( (m_status & WT_STARTED) ) { //-- Был на паузе или уже запущен
        if ( !_loopTimer->isActive() ) _loopTimer->start();
        m_status &= ~WT_PAUSED;
        m_status |= WT_RUNNING;
        return true;
    }

    QObject * curParent = parent();
    QThread * curThread = QThread::currentThread();

    setParent(nullptr);

    if ( _thread==nullptr ) _thread = new QThread(curParent); //-- Что бы сам удалился, без напоминаний
    moveToThread(_thread);

    //-- Как только будет запущен поток, запускаем выполнение Worker`а
    connect(_thread, &QThread::started, this, &WThread::process);

    //-- Если предок удалится, то нам то же нужно завершаться и удаляться
    if ( curParent!=nullptr ) {
        connect(curParent, &QObject::destroyed, [&](){ _thread->quit(); _thread->wait(); });
    } else
    if ( !delOnFinish ) {
        //qWarning()<<"WThread has no parent and delOnFinish not enabled!";
    }

    if ( delOnFinish ) {
        //-- Как только свистнет, что закончил - удаляемся гордо
        connect(this, &WThread::finished, this, &WThread::deleteLater, Qt::DirectConnection);
        connect(this, &WThread::finished, _thread, &QThread::quit, Qt::DirectConnection);
    } else {
        //-- Как только свистнет, что закончил - возвращаем поток откуда взяли, что бы предок отработал удаление
        WThread * worker = this;
        QThread * thread = _thread;
        connect(this, &WThread::finished, this, [worker, thread, curThread, curParent]() {
            thread->quit();
            worker->moveToThread(curThread);
            worker->setParent(curParent);
        }, Qt::DirectConnection);
    }

    //-- Сразу запускаем выполнение потока
    _thread->start(priority);

    m_status |= WT_STARTED;
    m_status |= WT_RUNNING;

    emit statusChanged(m_status);

    return true;
}

/**
* @brief Останавливаем выполнение. О полной остановке скажет сигнал finished();
* @return
*/
bool WThread::_stop()
{
    if ( !(m_status&WT_RUNNING) ) return false;

    if ( _eventLoop!=nullptr ) _eventLoop->quit();

    m_status &= ~WT_RUNNING;
    m_status &= ~WT_PAUSED;

    emit statusChanged(m_status);

    return true;
}

bool WThread::_pause()
{
    if ( !(m_status&WT_RUNNING) ) return false;

    _loopTimer->stop();

    m_status &= ~WT_RUNNING;
    m_status |= WT_PAUSED;

    emit statusChanged(m_status);

    return true;

}

void WThread::_process()
{
    assert(_thread!=nullptr);
    //-- Можем тут создавать классы-наследники, которые будут уже в нашем потоке
    bool isStarted = onStarted();
    if ( !isStarted ) { onFinished(); return; }

    //-- Создадим бесконечный типо цикл
    _loopTimer = new QTimer(this);
    connect(_loopTimer, &QTimer::timeout, this, &WThread::loop);

    //-- Как только завершились (да, без потока не будет и событий, но мы можем переместить в другой поток потом)
    connect(this, &WThread::finished, _loopTimer, &QTimer::stop);

    //-- QEventLoop перехватит грубо говоря очередь событый на себя и будет основной
    _eventLoop = new QEventLoop(this);
    //-- Запускаем таймер, что бы каждые 200мс запускал функцию

    bool isAfterStarted = onAfterStarted();
    if ( !isAfterStarted ) { stop(); onFinished();  return; }

    _loopTimer->start(3);
    //-- Ну и сразу запускаем нашу очередь, мы в ней будем, пока кто-нить quit/exit не вызовет
    _eventLoop->exec();

    //-- Если мы здесь оказались, значит _eventLoop завершился и точно, что пора выходить
    onFinished();
}

bool WThread::_onStarted()
{
    emit started();
    return true;
}

bool WThread::_onAfterStarted()
{
    return true;
}

void WThread::_onFinished()
{
    m_status &= ~WT_RUNNING;
    m_status &= ~WT_PAUSED;

    emit statusChanged(m_status);
    emit finished();
}
