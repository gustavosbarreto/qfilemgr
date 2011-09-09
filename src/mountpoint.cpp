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
