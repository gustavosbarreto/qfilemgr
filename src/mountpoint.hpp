#ifndef _MOUNTPOINT_HPP
#define _MOUNTPOINT_HPP

#include <QString>

class MountPoint
{
public:
    static QString mountPointForFilePath(const QString &filePath);
};

#endif
