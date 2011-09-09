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

#include "filesystemmodel.hpp"
#include "mimedetector.hpp"
#include "fileitem.hpp"
#include "filemanager.hpp"
#include "iconsviewcontroller.hpp"

#include <QTimer>
#include <QTime>

static QMap<QString, FileItem *> fileItems;

FileSystemModel::FileSystemModel(QObject *parent):
    QFileSystemModel(parent)
{
    mimeDetector = new MimeDetector("/usr/share/mime/mime.cache");
    qRegisterMetaType<FileItem *>("FileItem *");

    qDebug() << FileItem("/usr/share/applications/gedit.desktop").iconFilePath();

    connect(this, SIGNAL(layoutChanged()), SLOT(deleteUnusedFileItems()));
}

FileSystemModel::~FileSystemModel()
{
    delete mimeDetector;
}

QVariant FileSystemModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DecorationRole)
    {
        MimeType *mimeType = mimeDetector->mimeTypeForFileName(index.data(QFileSystemModel::FilePathRole).toString());
        return QIcon(mimeType->icon());
    }
    else if (role == MimeTypeDescriptionRole)
    {
        MimeType *mimeType = mimeDetector->mimeTypeForFileName(index.data(QFileSystemModel::FilePathRole).toString());
        return mimeType->description();
    }
    else if (role == MimeTypeIconRole)
    {
        MimeType *mimeType = mimeDetector->mimeTypeForFileName(index.data(QFileSystemModel::FilePathRole).toString());
        return mimeType->icon();
    }
    else if (role == DefaultApplicationNameRole)
    {
        MimeType *mimeType = mimeDetector->mimeTypeForFileName(index.data(QFileSystemModel::FilePathRole).toString());
        QList<MimeAction> actions = mimeType->actions();
        return actions.at(0).name();
    }
    else if (role == DefaultApplicationDesktopFileRole)
    {
        MimeType *mimeType = mimeDetector->mimeTypeForFileName(index.data(QFileSystemModel::FilePathRole).toString());
        QList<MimeAction> actions = mimeType->actions();
        return actions.at(0).desktopFile();
    }
    else if (role == DefaultApplicationIconRole)
    {
        MimeType *mimeType = mimeDetector->mimeTypeForFileName(index.data(QFileSystemModel::FilePathRole).toString());
        QList<MimeAction> actions = mimeType->actions();
        return actions.at(0).icon();
    }
    else if (role == FileItemRole)
    {
        QString filePath = index.data(QFileSystemModel::FilePathRole).toString();
        FileItem *fileItem = NULL;
        if (!(fileItem = fileItems.value(filePath)))
        {
            fileItem = new FileItem(filePath);
            fileItems.insert(filePath, fileItem);
        }

        return QVariant::fromValue(fileItem);
    }

    return QFileSystemModel::data(index, role);    
}

QModelIndex FileSystemModel::setRootPathOwn(const QString &newPath)
{
    QTimer::singleShot(0, this, SLOT(deleteUnusedFileItems()));

    QModelIndex index = setRootPath(newPath);

    FileManager::self()->iconsViewController()->selectItem(index);

    return index;
}

void FileSystemModel::deleteUnusedFileItems()
{
    static QList<FileItem *> toBeDeleted;

    while (!toBeDeleted.isEmpty())
    {
        FileItem *fileItem = toBeDeleted.takeFirst();
        QMap<QString, FileItem *>::iterator it = fileItems.find(fileItem->fileName());
        if (it != fileItems.end())
        {
            qDebug() << fileItem->fileName();
            fileItems.erase(it);
            delete fileItem;
        }
    }

    toBeDeleted = fileItems.values();
}
