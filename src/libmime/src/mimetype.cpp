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

#include "mimetype.hpp"
#include "mimeaction.hpp"
#include "mimecache.hpp"
#include "filemanager.hpp"
#include "mimeutils.hpp"

#include <QStringList>
#include <QDomDocument>
#include <QFile>
#include <QLocale>
#include <QSettings>

//#include <desktopentry.hpp>

MimeType::MimeType()
{
}

MimeType::MimeType(const QString mimeType):
    m_mimeType(mimeType)
{
    QStringList parts = mimeType.split(QRegExp("[/;]"));
    m_mediaType = parts.at(0);
    m_subType = parts.at(1);
}

MimeType::MimeType(const MimeType &other):
    m_mimeType(other.m_mimeType),
    m_mediaType(other.m_mediaType),
    m_subType(other.m_subType)
{
}

const QString MimeType::mimeType() const
{
    return m_mimeType;
}

const QString MimeType::mediaType() const
{
    return m_mediaType;
}

const QString MimeType::subType() const
{
    return m_subType;
}

const QString MimeType::description() const
{
    QMap<QString, QString> descriptions;

    QDomDocument doc;
    QFile file(QString("/usr/share/mime/%1.xml").arg(mimeType()));
    file.open(QFile::ReadOnly);
    doc.setContent(&file);
    file.close();

    QDomElement docElem = doc.documentElement();
    QDomNode commentNode = docElem.firstChild();
    while (!commentNode.isNull())
    {
        QDomElement e = commentNode.toElement();

        if (e.tagName() == "comment")
        {
            QString key = e.attributeNode("xml:lang").value();
            if (key.isEmpty())
                key = "c";
            descriptions.insert(key, e.text());
        }

        commentNode = commentNode.nextSibling();
    }

    QStringList whichList;
    whichList << QLocale::system().name() << QLocale::system().name().left(2) << "c";

    foreach (const QString &key, whichList)
    {
        QMap<QString, QString>::iterator it = descriptions.find(key);
        if (it != descriptions.end())
            return it.value();
    }

    return "";
}

const QString MimeType::icon() const
{
    return MIMEUtils::getIconForMimeType(mimeType(), mediaType());
}

QList<MimeAction> MimeType::actions() const
{
    QList<MimeAction> list;

#if 0
    QSettings defaultsList("/usr/share/applications/defaults.list", OSDesktopEntryFormat);
    defaultsList.beginGroup("Default Applications");
    if (!defaultsList.value(m_mimeType).isNull())
        list.append(MimeAction(defaultsList.value(m_mimeType).toString()));

    QSettings cache("/usr/share/applications/mimeinfo.cache", OSDesktopEntryFormat);
    cache.beginGroup("MIME Cache");
    foreach (const QString &entry, cache.value(m_mimeType).toString().split(';'))
    {
        if (entry.isEmpty())
            continue;
        list.append(MimeAction(entry));
    }

    if (list.isEmpty())
        list.append(MimeAction());
#endif
    return list;
}

bool MimeType::isSubclass(const QString &subclass) const
{
    MimeCache *cache = FileManager::self()->mimeCache();

    QString umimeType = cache->unaliasMimeType(mimeType());
    QString usubclass = cache->unaliasMimeType(subclass);

    MimeType tmpMimeType(umimeType);
    MimeType tmpSubclass(usubclass);

    if (umimeType == usubclass)
        return true;

    if (isSuperType() && tmpMimeType.mediaType() == tmpSubclass.mediaType())
        return true;

    if (usubclass == "text/plain" && tmpMimeType.mediaType() == "text")
        return true;

    if (usubclass == "application/octet-stream")
        return true;

    int listOffset = cache->parentListOffset();
    int listCount = cache->parentListCount(listOffset);

    for (int i = 0; i < listCount; ++i)
    {
        quint32 offset;

        cache->stream->device()->seek((listOffset + 4) + (i * 8));
        *cache->stream >> offset;

        if (cache->getString(offset) == umimeType)
        {
/*            cache->stream->device()->seek((listOffset + 4) + (i * 8) + 4);
 *cache->stream >> offset;*/
        }
    }

}

bool MimeType::isSuperType() const
{
    return (mimeType().right(2) == "/*");
}
