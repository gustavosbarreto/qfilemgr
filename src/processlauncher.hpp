#ifndef _PROCESSLAUNCHER_HPP
#define _PROCESSLAUNCHER_HPP

#include <QStringList>

class ProcessLauncher
{
public:
    static bool startDetached(const QString &program, const QStringList &arguments);
};

#endif
