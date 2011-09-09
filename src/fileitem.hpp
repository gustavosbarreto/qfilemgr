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

#ifndef _FILEITEM_HPP
#define _FILEITEM_HPP

#include <QMetaType>
#include <QSharedData>

class FileItemData;

class FileItem
{
public:
    FileItem(const QString &fileName);
    FileItem(const FileItem &other);
    ~FileItem();

    bool exists() const;
    bool isDir() const;

    quint64 size() const;

    const QString fileName() const;
    const QString filePath() const;

    const QString iconFilePath();

    const QString mimeType() const;
    const QString mimeMediaType() const;
    const QString mimeSubtype() const;

private:
    QSharedDataPointer<FileItemData> d;
};

Q_DECLARE_METATYPE(FileItem *)

#endif
