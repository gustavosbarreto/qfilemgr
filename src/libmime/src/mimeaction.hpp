#ifndef _MIMEACTION_HPP
#define _MIMEACTION_HPP

#include <QString>

class MimeAction
{
public:
    MimeAction();
    MimeAction(const QString &fileName);
    MimeAction(const MimeAction &other);

    bool isValid() const;

    const QString desktopFile() const;
    const QString name() const;
    const QString comment() const;
    const QString exec() const;
    const QString mimeType() const;
    const QString icon() const;

private:
    bool m_valid;
    QString m_desktopFile;
    QString m_name;
    QString m_comment;
    QString m_exec;
    QString m_mimeType;
    QString m_icon;
};

#endif
