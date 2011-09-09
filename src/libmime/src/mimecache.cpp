#include "mimecache.hpp"

#include <QFile>
#include <QDataStream>
#include <QDebug>

MimeCache::MimeCache(const QString &fileName)
{
    QFile file(fileName);
    file.open(QFile::ReadOnly);

    stream = new QDataStream(new QByteArray(file.readAll()), QIODevice::ReadOnly);
}

QString MimeCache::aliasLookup(const QString &alias)
{
    int listOffset = aliasListOffset();
    int listCount = aliasListCount(listOffset);
    for (int i = 0; i < listCount; ++i)
    {
        quint32 aliasOffset;
        quint32 mimeOffset;

        stream->device()->seek((listOffset + 4) + (i * 8));
        *stream >> aliasOffset;

        stream->device()->seek((listOffset + 4) + (i * 8) + 4);
        *stream >> mimeOffset;

        if (getString(aliasOffset) == alias)
        {
            stream->device()->seek((listOffset + 4) + (i * 8) + 4);
            *stream >> mimeOffset;
            return getString(mimeOffset);
        }
    }

    return "";
}

QString MimeCache::unaliasMimeType(const QString &mimeType)
{
    QString lookup = aliasLookup(mimeType);
    return lookup.isEmpty() ? mimeType : lookup;
}

quint32 MimeCache::aliasListOffset()
{
    quint32 value;
    stream->device()->reset();
    stream->device()->seek(4);
    *stream >> value;
    return value;
}

quint32 MimeCache::aliasListCount(int offset)
{
    quint32 value;
    stream->device()->reset();
    stream->device()->seek(offset);
    *stream >> value;
    return value;
}


QString MimeCache::getString(int offset, bool regExp)
{
    qint64 pos = stream->device()->pos();
    stream->device()->seek(offset);

    QString str;

    char c = 0;
    stream->device()->getChar(&c);
    while (c != 0)
    {
        if (regExp)
        {
            switch (c)
            {
                case '.':
                    str.append("\\");
                    break;
                case '*':
                case '+':
                case '?':
                    str.append(".");
            }
        }

        str.append(c);
        stream->device()->getChar(&c);
    }

    stream->device()->reset();
    stream->device()->seek(pos);

    return str;
}

quint32 MimeCache::parentListOffset()
{
    quint32 value;
    stream->device()->reset();
    stream->device()->seek(8);
    *stream >> value;
    return value;
}

quint32 MimeCache::parentListCount(int offset)
{
    quint32 value;
    stream->device()->reset();
    stream->device()->seek(offset);
    *stream >> value;
    return value;
}

quint32 MimeCache::iconListOffset()
{
    quint32 value;
    stream->device()->reset();
    stream->device()->seek(36);
    *stream >> value;
    return value;
}

quint32 MimeCache::iconListCount(int offset)
{
    quint32 value;
    stream->device()->reset();
    stream->device()->seek(offset);
    *stream >> value;
    return value;
}

