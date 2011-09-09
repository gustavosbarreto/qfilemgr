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
