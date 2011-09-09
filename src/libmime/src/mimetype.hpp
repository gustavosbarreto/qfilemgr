#ifndef _MIMETYPE_HPP
#define _MIMETYPE_HPP

#include <QString>
#include <QList>
#include <QDebug>

#include "mimeaction.hpp"

class MimeType
{
public:
    MimeType();
    MimeType(const QString mimeType);
    MimeType(const MimeType &other);

    const QString mimeType() const;
    const QString mediaType() const;
    const QString subType() const;
    const QString description() const;
    const QString icon() const;
    QList<MimeAction> actions() const;

    bool isSubclass(const QString &subclass) const;

private:
    bool isSuperType() const;

private:
    QString m_mimeType;
    QString m_mediaType;
    QString m_subType;
};

inline QDebug operator<<(QDebug dbg, const MimeType &mime)
{
    dbg.nospace() << "MimeType(mimeType=" << mime.mimeType() << ", mediaType=" << mime.mediaType() << ", subType=" << mime.subType() << ")";
    return dbg.space();
}

#endif
