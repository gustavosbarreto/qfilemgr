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

#ifndef _ICONENGINE_HPP
#define _ICONENGINE_HPP

#include <QString>

class FileItem;

class IconEngine
{
    enum Context
    {
        AnyContext,
        ApplicationContext,
        MimeTypeContext
    };

public:
    IconEngine(const QString &iconTheme, int iconSize);

    QString applicationIcon(const QString &iconName);
    QString mimeTypeIcon(const QString &mimeType, const QString &mediaType);

private:
    QString contextIconPath(Context context);

private:
    QString m_iconTheme;
    int m_iconSize;
};

#endif
