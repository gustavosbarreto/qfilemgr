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

#include "mimeutils.hpp"

#include <QFile>
#include <QMap>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

static QMap<QString, QString> genericIcons;
static QMap<QString, QString> iconList;

namespace MIMEUtils
{
    const QString getIconForMimeType(const QString &mimeType, const QString &mediaType)
    {
        if (genericIcons.isEmpty())
        {
            QFile file("/usr/share/mime/generic-icons");
            file.open(QFile::ReadOnly | QIODevice::Text);

            QTextStream stream(&file);
            while (!stream.atEnd())
            {
                QStringList parts = stream.readLine().split(':');
                genericIcons.insert(parts.at(0), parts.at(1));
            }

            file.close();
        }

        QMap<QString, QString>::iterator it = iconList.find(mimeType);
        if (it == iconList.end())
        {
            QString prefix = "/usr/share/icons/oxygen/48x48/mimetypes/";
            QString icon = QString(mimeType).replace('/', '-') + ".png"; // Adaptacao geral aos nomes de icones

            if (!QFile::exists(prefix + icon))
            {
                // Adapta aos nomes usado nos icones do KDE
                if (mediaType == "image")
                    icon = "image-x-generic.png";
                else if (mediaType == "video")
                    icon = "video-x-generic.png";
                else if (mediaType == "audio")
                    icon = "audio-x-generic.png";
            }

            // Icones genericos
            if (!QFile::exists(prefix + icon))
            {
                if (!genericIcons.value(mimeType).isEmpty())
                    icon = genericIcons.value(mimeType) + ".png";
            }

            if (!QFile::exists(prefix + icon))
                icon = "unknown";

            return iconList.insert(mimeType, prefix + icon).value();
        }
        else
        {
            return it.value();
        }
    }
}
