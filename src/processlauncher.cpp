#include "processlauncher.hpp"
#include "startupnotifyhandler.hpp"

#include <QProcess>
#include <QApplication>
#include <QCursor>
#include <QX11Info>
#include <QDebug>

#include <stdlib.h>
//#include <libsn/sn.h>

#if 0
static QCoreApplication::EventFilter oldX11EventFilter = NULL;

static SnLauncherContext *snLauncher = NULL;
static SnMonitorContext *snMonitor = NULL;
static SnDisplay *snDisplay = NULL;

static bool x11EventFilter(void *message, long *result)
{
    XEvent *event = reinterpret_cast<XEvent *>(message);
    sn_display_process_event(snDisplay, event);

    if (oldX11EventFilter && oldX11EventFilter != &x11EventFilter)
        return oldX11EventFilter(message, result);
    else
        return false;
}

static void startupNotificationMonitor(SnMonitorEvent *ev, void *data)
{
    SnStartupSequence *seq;
    if (!(seq = sn_monitor_event_get_startup_sequence(ev)) || !data)
        return;

    StartupNotifyHandler *startupWaits = static_cast<StartupNotifyHandler *>(data);
    const char *id = sn_startup_sequence_get_id(seq);

    switch (sn_monitor_event_get_type(ev))
    {
        case SN_MONITOR_EVENT_INITIATED:
            startupWaits->add(id);
            QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
            break;

        case SN_MONITOR_EVENT_CHANGED:
            break;

        case SN_MONITOR_EVENT_COMPLETED:
        case SN_MONITOR_EVENT_CANCELED:
            startupWaits->remove(id);
            QApplication::restoreOverrideCursor();
            break;
    }
}

static void startupNotificationInit()
{
    oldX11EventFilter = QCoreApplication::instance()->setEventFilter(x11EventFilter);

    int screen = QX11Info().screen();

    snDisplay = sn_display_new(QX11Info::display(), NULL, NULL);
    snMonitor = sn_monitor_context_new(snDisplay, screen, &startupNotificationMonitor, new StartupNotifyHandler, NULL);
    snLauncher = sn_launcher_context_new(snDisplay, screen);
}
#endif

bool ProcessLauncher::startDetached(const QString &program, const QStringList &arguments)
{
    #if 0
    if (!snDisplay)
        startupNotificationInit();
    #endif

    QString commandString = program + " " + arguments.join(" ");

    #if 0
    if (sn_launcher_context_get_initiated(snLauncher))
    {
        sn_launcher_context_unref(snLauncher);
        snLauncher = sn_launcher_context_new(snDisplay, QX11Info().screen());
    }

    sn_launcher_context_initiate(snLauncher, program.toUtf8().constData(), commandString.toUtf8().constData(), CurrentTime);

    const char *id = sn_launcher_context_get_startup_id(snLauncher);

    setenv("DESKTOP_STARTUP_ID", id, TRUE);
    // for OS' qt applications, such as ostdm
    setenv("OS_DESKTOP_STARTUP_ID", id, TRUE);
    #endif

    return QProcess::startDetached(program, arguments);
}
