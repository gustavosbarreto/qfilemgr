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

#ifndef _FILESYSTEMMODEL_HPP
#define _FILESYSTEMMODEL_HPP

#include <QFileSystemModel>

class MimeDetector;
class FileItem;

class FileSystemModel: public QFileSystemModel
{
    Q_OBJECT

public:
    enum Role
    {
        MimeTypeDescriptionRole = Qt::UserRole + 4, // Descricao do MimeType
        MimeTypeIconRole, // Icone do MimeType
        DefaultApplicationNameRole, // Nome do aplicativo padrao que abre o arquivo
        DefaultApplicationDesktopFileRole, // Arquivo .desktop do aplicativo padrao que abre o arquivo
        DefaultApplicationIconRole, // Icone do aplicativo padrao que abre o arquivo
        FileItemRole // MAGIC
    };

    FileSystemModel(QObject *parent = 0);
    ~FileSystemModel();

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QModelIndex setRootPathOwn(const QString &newPath);

private slots:
    void deleteUnusedFileItems();

private:
    MimeDetector *mimeDetector;
};

#endif
