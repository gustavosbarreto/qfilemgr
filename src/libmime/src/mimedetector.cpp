/*
 *  Copyright (C) 2011 O.S. Systems Softwares Ltda. All rights reserved.
 *  Authors:
 *    Luis Gustavo S. Barreto <gustavosbarreto@gmail.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "mimedetector.hpp"
#include "mimecache.hpp"
#include "filemanager.hpp"

#include <QFile>
#include <QDataStream>
#include <QMap>
#include <QFileInfo>

MimeDetector::MimeDetector(const QString mimeCacheFile)
{
    stream = FileManager::self()->mimeCache()->stream;
}

MimeDetector::~MimeDetector()
{
    foreach (MimeType *mimeType, m_mimeTypess.values())
        delete m_mimeTypess.take(mimeType->mimeType());
}

MimeType *MimeDetector::mimeTypeForFileName(const QString &fileName)
{
    QStringList list;

    if (QFileInfo(fileName).isDir())
        list.append("inode/directory");
    else
        list.append(lookupMimeTypesForGlobFileName(fileName));

    if (list.isEmpty())
        list.append("application/octet-stream");

    if (list.size() > 1)
        qWarning("list.size() > 1");

    MimeType *mimeType = m_mimeTypess.value(list.at(0));
    if (!mimeType)
    {
        mimeType = new MimeType(list.at(0));
        m_mimeTypess.insert(list.at(0), mimeType);
    }

    return mimeType;
}

QStringList MimeDetector::lookupMimeTypesForGlobFileName(const QString &fileName)
{
    QStringList list = lookupGlobLiteral(fileName);
    if (!list.isEmpty())
        return list;

    list = lookupGlobSuffix(fileName, false, fileName.length());
    if (!list.isEmpty())
        return list;

    list = lookupGlobSuffix(fileName, true, fileName.length());
    if (!list.isEmpty())
        return list;

    list = lookupGlobFileNameMatch(fileName);
    return list;
}

QStringList MimeDetector::lookupGlobLiteral(const QString &fileName)
{
    QStringList list;

    int listOffset = literalListOffset();
    int numEntries = literalListCount(listOffset);

    for (int i = 0; i < numEntries; ++i)
    {
        quint32 offset;
        stream->device()->reset();
        stream->device()->seek((listOffset + 4) + (i * 12));
        *stream >> offset;

        QString literal = getString(offset);
        if (literal == fileName)
        {
            stream->device()->reset();
            stream->device()->seek((listOffset + 4) + (i * 12) + 4);
            *stream >> offset;
            QString mimeType = getString(offset);

            quint32 weight;
            stream->device()->reset();
            stream->device()->seek((listOffset + 4) + (i * 12) + 8);
            *stream >> weight;

            list.append(mimeType);
        }
    }

    return list;
}

QStringList MimeDetector::lookupGlobSuffix(QString fileName, bool ignoreCase, int len)
{
    QStringList list;

    int listOffset = reverseSuffixTreeOffset();
    int numEntries = reverseSuffixTreeCount(listOffset);

    quint32 offset;
    stream->device()->reset();
    stream->device()->seek(listOffset + 4);
    *stream >> offset;

    list += lookupGlobNodeSuffix(fileName, numEntries, offset, ignoreCase, len);

    return list;
}

QStringList MimeDetector::lookupGlobFileNameMatch(const QString &fileName)
{
    QStringList list;

    int listOffset = globListOffset();
    int numEntries = globListCount(listOffset);
    for (int i = 0; i < numEntries; ++i)
    {
        quint32 offset;
        stream->device()->reset();
        stream->device()->seek((listOffset + 4) + (12 * i));
        *stream >> offset;

        quint32 mimeTypeOffset;
        stream->device()->reset();
        stream->device()->seek((listOffset + 4) + (12 * i) + 4);
        *stream >> mimeTypeOffset;

        quint32 weight;
        stream->device()->reset();
        stream->device()->seek((listOffset + 4) + (12 * i) + 8);
        *stream >> weight;

        QString pattern = getString(offset, true);
        QString mimeType = getString(mimeTypeOffset);

        if (QRegExp(pattern).exactMatch(fileName))
            list.append(mimeType);
    }

    return list;
}

QStringList MimeDetector::lookupGlobNodeSuffix(const QString &fileName,
                                                           int numEntries, int offset,
                                                           bool ignoreCase, int len, QString pattern)
{
    QStringList list;

    char character = ignoreCase ? fileName.toLower().at(len - 1).toLatin1() : fileName.at(len - 1).toLatin1();

    for (int i = 0; i < numEntries; ++i)
    {
        quint32 matchChar;
        stream->device()->reset();
        stream->device()->seek(offset + (12 * i));
        *stream >> matchChar;

        if ((char)matchChar != character)
            continue;

        len--;

        quint32 numChildren;
        stream->device()->seek(offset + (12 * i) + 4);
        *stream >> numChildren;

        quint32 childOffset;
        stream->device()->seek(offset + (12 * i) + 8);
        *stream >> childOffset;

        if (len > 0)
        {
            pattern.prepend(matchChar);
            list += lookupGlobNodeSuffix(fileName, numChildren, childOffset, ignoreCase, len, pattern);
        }

        if (!list.isEmpty())
            continue;

        for (int j = 0; j < (int)numChildren; ++j)
        {
            stream->device()->reset();
            stream->device()->seek(childOffset + (12 * j));
            *stream >> matchChar;
            if (matchChar != 0)
                break;

            quint32 mimeOffset;
            stream->device()->reset();
            stream->device()->seek(childOffset + (12 * j) + 4);
            *stream >> mimeOffset;

            QString mimeType = getString(mimeOffset);

            quint32 weight;
            stream->device()->reset();
            stream->device()->seek(childOffset + (12 * j) + 8);
            *stream >> weight;

            list.append(mimeType);
        }

        break;
    }

    return list;
}

void MimeDetector::initMimeTypes()
{
    int listOffset = aliasListOffset();
    int numAliases = aliasesCount(listOffset);
    for (int i = 0; i < numAliases; ++i)
    {
        quint32 offset;

        stream->device()->seek((listOffset + 4) + (i * 8));
        *stream >> offset;
//        qDebug() << offset;

        addMimeType(MimeType(getString(offset)));

        stream->device()->seek((listOffset + 8) + (i * 8));
        *stream >> offset;
        addMimeType(MimeType(getString(offset)));
    }
}

void MimeDetector::addMimeType(const MimeType &mimeType)
{
    QMap<QString, QStringList>::iterator it = m_mimeTypes.find(mimeType.mediaType());
    if (it == m_mimeTypes.end())
        m_mimeTypes.insert(mimeType.mediaType(), QStringList() << mimeType.subType());
    else
        it.value() << mimeType.subType();
}

QString MimeDetector::getString(int offset, bool regExp)
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

quint32 MimeDetector::aliasListOffset()
{
    quint32 value;
    stream->device()->reset();
    stream->device()->seek(4);
    *stream >> value;
    return value;
}

quint32 MimeDetector::aliasesCount(int offset)
{
    quint32 value;
    stream->device()->reset();
    stream->device()->seek(offset);
    *stream >> value;
    return value;
}

quint32 MimeDetector::literalListOffset()
{
    quint32 value;
    stream->device()->reset();
    stream->device()->seek(12);
    *stream >> value;
    return value;
}

quint32 MimeDetector::literalListCount(int offset)
{
    quint32 value;
    stream->device()->reset();
    stream->device()->seek(offset);
    *stream >> value;
    return value;
}

quint32 MimeDetector::reverseSuffixTreeOffset()
{
    quint32 value;
    stream->device()->reset();
    stream->device()->seek(16);
    *stream >> value;
    return value;
}

quint32 MimeDetector::reverseSuffixTreeCount(int offset)
{
    quint32 value;
    stream->device()->reset();
    stream->device()->seek(offset);
    *stream >> value;
    return value;
}

quint32 MimeDetector::globListOffset()
{
    quint32 value;
    stream->device()->reset();
    stream->device()->seek(20);
    *stream >> value;
    return value;
}

quint32 MimeDetector::globListCount(int offset)
{
    quint32 value;
    stream->device()->reset();
    stream->device()->seek(offset);
    *stream >> value;
    return value;
}
