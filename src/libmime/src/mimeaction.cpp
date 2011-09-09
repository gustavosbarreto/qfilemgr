#include "mimeaction.hpp"

#include <QSettings>
#include <QDebug>
#include <QStringList>
#include <QFile>

//#include <desktopentry.hpp>

MimeAction::MimeAction()
{
    m_name = QObject::trUtf8("Unknown");
    m_icon = "/usr/share/icons/oxygen/48x48/mimetypes/application-x-desktop.png";
}

MimeAction::MimeAction(const QString &fileName):
    m_valid(true)
{
    m_desktopFile = QString("/usr/share/applications/%1").arg(fileName);

#if 0
    QSettings s(QString("/usr/share/applications/%1").arg(fileName), OSDesktopEntryFormat);
    s.beginGroup("Desktop Entry");
    m_name = s.value("Name").toString();
    m_comment = s.value("Comment").toString();
    m_exec = s.value("Exec").toString();
    m_mimeType = s.value("MimeType").toString();

    QStringList extensions;
    extensions << "png" << "xpm";
    foreach (const QString &extension, extensions)
    {
        m_icon = "/usr/share/pixmaps/" + s.value("Icon").toString() + "." + extension;
        if (QFile::exists(m_icon))
            break;
    }

    if (!QFile::exists(m_icon))
        m_icon = "/usr/share/icons/oxygen/48x48/mimetypes/application-x-desktop.png";
#endif
}

MimeAction::MimeAction(const MimeAction &other):
    m_valid(other.m_valid),
    m_desktopFile(other.m_desktopFile),
    m_name(other.m_name),
    m_comment(other.m_comment),
    m_exec(other.m_exec),
    m_mimeType(other.m_mimeType),
    m_icon(other.m_icon)
{
}

bool MimeAction::isValid() const
{
    return m_valid;
}

const QString MimeAction::desktopFile() const
{
    return m_desktopFile;
}

const QString MimeAction::name() const
{
    return m_name;
}

const QString MimeAction::comment() const
{
    return m_comment;
}

const QString MimeAction::exec() const
{
    return m_exec;
}

const QString MimeAction::mimeType() const
{
    return m_mimeType;
}

const QString MimeAction::icon() const
{
    return m_icon;
}
