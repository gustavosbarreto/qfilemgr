#ifndef _VOLUMEFREESPACEINFO_HPP
#define _VOLUMEFREESPACEINFO_HPP

#include <QSharedData>

class VolumeFreeSpaceInfo
{
public:
    VolumeFreeSpaceInfo(const VolumeFreeSpaceInfo &other);
    ~VolumeFreeSpaceInfo();

    quint64 size() const;
    quint64 available() const;
    quint64 used() const;

    static VolumeFreeSpaceInfo freeSpaceInfo(const QString &mountPoint);

private:
    VolumeFreeSpaceInfo();

private:
    class Data;
    QSharedDataPointer<Data> d;
};

#endif
