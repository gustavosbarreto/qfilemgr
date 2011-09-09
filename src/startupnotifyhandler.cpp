/*
    WARNING: this code is duplicated by osthindesktop-desk project.
    Therefore, any changes you make in this file must be applied in it's duplicata.
*/

#include <QApplication>
#include <QTimer>

#include "startupnotifyhandler.hpp"


StartupNotifyHandler::StartupNotifyHandler(QObject *parent):
    QObject(parent)
{
}

void StartupNotifyHandler::add(const QString &id, void *data)
{
    QTimer *timer = new QTimer;
    timer->start(TIMEOUT);
    connect(timer, SIGNAL(timeout()), SLOT(onTimeout()));

    m_startup << StartupData(id, timer, data);
}

void StartupNotifyHandler::remove(const QString &id)
{
    QList<StartupData>::Iterator walker;
    for (walker = m_startup.begin(); walker != m_startup.end(); walker++)
    {
        if (walker->id == id)
        {
            walker->timer->stop();
            walker->timer->deleteLater();
            m_startup.erase(walker);
            return;
        }
    }
}

bool StartupNotifyHandler::contains(const QString &id)
{
    QList<StartupData>::Iterator walker;
    for (walker = m_startup.begin(); walker != m_startup.end(); walker++)
    {
        if (walker->id == id)
            return true;
    }

    return false;
}

void *StartupNotifyHandler::getUserData(const QString &id)
{
    QList<StartupData>::Iterator walker;
    for (walker = m_startup.begin(); walker != m_startup.end(); walker++)
    {
        if (walker->id == id)
            return walker->userData;
    }

    return NULL;
}

void StartupNotifyHandler::print()
{
    QList<StartupData>::Iterator walker;
    for (walker = m_startup.begin(); walker != m_startup.end(); walker++)
        qDebug("[%s, %p, %p]", qPrintable(walker->id), walker->timer, walker->userData);
}

void StartupNotifyHandler::onTimeout()
{
    if (QObject::sender())
    {
        QTimer *timer = qobject_cast<QTimer *>(QObject::sender());
        QString id;

        foreach (StartupData data, m_startup)
            if (data.timer == timer)
                id = data.id;

        remove(id);
        QApplication::restoreOverrideCursor();
    }
}


StartupNotifyHandler::StartupData::StartupData(const QString &_id, QTimer *t, void *data):
    id(_id),
    timer(t),
    userData(data)
{
}
