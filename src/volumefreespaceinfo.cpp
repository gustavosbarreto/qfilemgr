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
