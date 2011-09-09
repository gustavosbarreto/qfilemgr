#ifndef _TRASHINFO_HPP
#define _TRASHINFO_HPP

#include <QString>
#include <QDateTime>

class TrashInfo
{
public:
    TrashInfo(const QString &filePath);

    QString path() const;
    QDateTime deletionDate() const;

private:
    QString m_path;
    QDateTime m_deletionDate;
};

#endif
