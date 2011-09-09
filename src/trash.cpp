#include "trash.hpp"

#include <QDir>

Trash::Trash(QObject *parent):
    QObject(parent)
{
}

QString Trash::trashPath()
{
    QString path = QString::fromUtf8(qgetenv("XDG_DATA_HOME"));
    if (!path.isEmpty())
        return path.append("Trash");
    else
        return path.append(QDir::homePath()).append("/.local/share/Trash");
}
