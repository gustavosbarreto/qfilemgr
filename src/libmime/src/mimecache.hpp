#ifndef _MIMECACHE_HPP
#define _MIMECACHE_HPP

#include <QString>

class QDataStream;

class MimeCache
{
public:
    MimeCache(const QString &fileName);

    QString aliasLookup(const QString &alias);
    QString unaliasMimeType(const QString &mimeType);

    QString getString(int offset, bool regExp = false);

    quint32 aliasListOffset();
    quint32 aliasListCount(int offset);

    quint32 parentListOffset();
    quint32 parentListCount(int offset);

    quint32 iconListOffset();
    quint32 iconListCount(int offset);

    QDataStream *stream;
};

#endif
