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

#ifndef _FILEMANAGER_HPP
#define _FILEMANAGER_HPP

#include <QObject>
#include <QMap>
#include <QStringList>
#include <QSize>

class MainWindow;
class FileSystemModel;
class PlacesModel;
class IconsViewController;
class HistoryList;
class MimeCache;
class IconEngine;
class DeviceManager;
class QModelIndex;

class FileManager: public QObject
{
    Q_OBJECT

public:
    FileManager();

    MainWindow *mainWindow() const;
    FileSystemModel *fileSystemModel() const;
    PlacesModel *placesModel() const;
    IconsViewController *iconsViewController() const;
    HistoryList *historyList() const;
    MimeCache *mimeCache() const;
    IconEngine *iconEngine() const;

    QString pasteFile(const QString &source, const QString &destination, bool cut = false);
    bool removeFile(const QString &fileName);
    QString newFolder(const QString &workingDirectory);
    QString renameFile(const QString &fileName);

    static FileManager *self();

private slots:
    void emitRootPathChanged(const QString &path);

signals:
    void rootPathChanged(const QModelIndex &);
};

#endif
