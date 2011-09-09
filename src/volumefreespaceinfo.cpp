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

#include "volumefreespaceinfo.hpp"

#include <QFile>

#include <sys/statvfs.h>

class VolumeFreeSpaceInfo::Data: public QSharedData
{
public:
    Data(): QSharedData() { }
    Data(const Data &other):
        QSharedData(other),
        size(other.size),
        available(other.available),
        used(other.used)
    {
    }
    ~Data() { }

    quint64 size;
    quint64 available;
    quint64 used;
};

VolumeFreeSpaceInfo::VolumeFreeSpaceInfo(const VolumeFreeSpaceInfo &other):
    d(other.d)
{
}

VolumeFreeSpaceInfo::~VolumeFreeSpaceInfo()
{
}

quint64 VolumeFreeSpaceInfo::size() const
{
    return d->size;
}

quint64 VolumeFreeSpaceInfo::available() const
{
    return d->available;
}

quint64 VolumeFreeSpaceInfo::used() const
{
    return d->used;
}

VolumeFreeSpaceInfo VolumeFreeSpaceInfo::freeSpaceInfo(const QString &mountPoint)
{
    VolumeFreeSpaceInfo info;

    struct statvfs statvfs_buf;

    if (!statvfs(QFile::encodeName(mountPoint).constData(), &statvfs_buf)) {
        const quint64 blksize = (quint64)statvfs_buf.f_bsize;
        info.d->available = statvfs_buf.f_bavail * blksize;
        info.d->size = statvfs_buf.f_blocks * blksize;
    }

    return info;
}

VolumeFreeSpaceInfo::VolumeFreeSpaceInfo()
{
    d = new Data;
}
