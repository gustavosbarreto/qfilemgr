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

#include "fileitem.hpp"
#include "filemanager.hpp"
#include "iconengine.hpp"

#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

//#include <desktopentry.hpp>

class FileItemData: public QSharedData
{
public:
    FileItemData(): QSharedData() { }
    FileItemData(const FileItemData &other):
        QSharedData(other),
        fileName(other.fileName),
        iconFilePath(other.iconFilePath),
        mimeType(other.mimeType),
        mimeMediatype(other.mimeMediatype),
        mimeSubtype(other.mimeSubtype)
    {
    }
    ~FileItemData() { }

    QString fileName;
    QString iconFilePath;
    QString mimeType;
    QString mimeMediatype;
    QString mimeSubtype;
};

FileItem::FileItem(const QString &fileName)
{
    d = new FileItemData;
    d->fileName = fileName;
}

FileItem::FileItem(const FileItem &other):
    d(other.d)
{
}

FileItem::~FileItem()
{
}

bool FileItem::exists() const
{
    return QFile::exists(d->fileName);
}

bool FileItem::isDir() const
{
    return QFileInfo(d->fileName).isDir();
}

quint64 FileItem::size() const
{
    return QFileInfo(d->fileName).size();
}

const QString FileItem::fileName() const
{
    return d->fileName;
}

const QString FileItem::filePath() const
{
    return QFileInfo(d->fileName).path();
}

const QString FileItem::iconFilePath()
{
    if (!d->iconFilePath.isEmpty())
        return d->iconFilePath; // Icone cacheado

    // Especifico para um .desktop
    if (mimeType() == "application/x-desktop")
    {
        #if 0
        QSettings desktopEntry(fileName(), OSDesktopEntryFormat);
        QString iconName = desktopEntry.value("Desktop Entry/Icon").toString();

        d->iconFilePath = FileManager::self()->iconEngine()->applicationIcon(iconName);

        // Se nao encontrar o icone no contexto 'Application' tenta o mimetype
        if (d->iconFilePath.isEmpty() || !QFile::exists(d->iconFilePath))
            d->iconFilePath = FileManager::self()->iconEngine()->mimeTypeIcon(mimeType(), mimeMediaType());
        #endif
    }
    else
        d->iconFilePath = FileManager::self()->iconEngine()->mimeTypeIcon(mimeType(), mimeMediaType());

    return d->iconFilePath;
}

const QString FileItem::mimeType() const
{
    return "application/x-desktop";
}

const QString FileItem::mimeMediaType() const
{
    return "application";
}

const QString FileItem::mimeSubtype() const
{
}
