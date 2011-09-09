/*
    WARNING: this code is duplicated by osthindesktop-desk project.
    Therefore, any changes you make in this file must be applied in it's duplicata.
*/

#ifndef _STARTUPNOTIFYHANDLER_HPP
#define _STARTUPNOTIFYHANDLER_HPP

#include <QString>
#include <QObject>
#include <QList>

#define TIMEOUT 20000 // timeout in ms

class QTimer;


class StartupNotifyHandler: public QObject
{
    Q_OBJECT

public:
    StartupNotifyHandler(QObject *parent = 0);

    void add(const QString &id, void *data = NULL);
    void remove(const QString &id);
    bool contains(const QString &id);
    void *getUserData(const QString &id);
    void print();

private:
    struct StartupData
    {
        QString id;
        QTimer *timer;
        void *userData;

        StartupData(const QString &_id, QTimer *t, void *data = NULL);
    };

    QList<StartupData> m_startup;

private slots:
    void onTimeout();
};

#endif
