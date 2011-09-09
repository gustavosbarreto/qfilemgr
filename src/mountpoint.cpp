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

#include "mountpoint.hpp"

#include <QStringList>
#include <QDebug>

#include <sys/statvfs.h>
#include <mntent.h>

QString MountPoint::mountPointForFilePath(const QString &filePath)
{
    QStringList list;

    FILE *mnttab = setmntent(MOUNTED, "r");

    struct mntent *mnt;
    while ((mnt = getmntent(mnttab)))
        list.append(mnt->mnt_dir);

    endmntent(mnttab);

    int max = 0;
    QString mountPoint;
    foreach (const QString &mp, list)
    {
        const int length = mp.length();
        if (filePath.startsWith(mp) && length > max)
        {
            max = length;
            mountPoint = mp;
        }
    }

    return mountPoint;
}
