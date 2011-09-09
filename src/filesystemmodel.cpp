#include "filesystemmodel.hpp"
#include "mimedetector.hpp"
#include "fileitem.hpp"
#include "filemanager.hpp"
#include "iconsviewcontroller.hpp"

#include <QTimer>
#include <QTime>

static QMap<QString, FileItem *> fileItems;

FileSystemModel::FileSystemModel(QObject *parent):
    QFileSystemModel(parent)
{
    mimeDetector = new MimeDetector("/usr/share/mime/mime.cache");
    qRegisterMetaType<FileItem *>("FileItem *");

    qDebug() << FileItem("/usr/share/applications/gedit.desktop").iconFilePath();

    connect(this, SIGNAL(layoutChanged()), SLOT(deleteUnusedFileItems()));
}

FileSystemModel::~FileSystemModel()
{
    delete mimeDetector;
}

QVariant FileSystemModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DecorationRole)
    {
        MimeType *mimeType = mimeDetector->mimeTypeForFileName(index.data(QFileSystemModel::FilePathRole).toString());
        return QIcon(mimeType->icon());
    }
    else if (role == MimeTypeDescriptionRole)
    {
        MimeType *mimeType = mimeDetector->mimeTypeForFileName(index.data(QFileSystemModel::FilePathRole).toString());
        return mimeType->description();
    }
    else if (role == MimeTypeIconRole)
    {
        MimeType *mimeType = mimeDetector->mimeTypeForFileName(index.data(QFileSystemModel::FilePathRole).toString());
        return mimeType->icon();
    }
    else if (role == DefaultApplicationNameRole)
    {
        MimeType *mimeType = mimeDetector->mimeTypeForFileName(index.data(QFileSystemModel::FilePathRole).toString());
        QList<MimeAction> actions = mimeType->actions();
        return actions.at(0).name();
    }
    else if (role == DefaultApplicationDesktopFileRole)
    {
        MimeType *mimeType = mimeDetector->mimeTypeForFileName(index.data(QFileSystemModel::FilePathRole).toString());
        QList<MimeAction> actions = mimeType->actions();
        return actions.at(0).desktopFile();
    }
    else if (role == DefaultApplicationIconRole)
    {
        MimeType *mimeType = mimeDetector->mimeTypeForFileName(index.data(QFileSystemModel::FilePathRole).toString());
        QList<MimeAction> actions = mimeType->actions();
        return actions.at(0).icon();
    }
    else if (role == FileItemRole)
    {
        QString filePath = index.data(QFileSystemModel::FilePathRole).toString();
        FileItem *fileItem = NULL;
        if (!(fileItem = fileItems.value(filePath)))
        {
            fileItem = new FileItem(filePath);
            fileItems.insert(filePath, fileItem);
        }

        return QVariant::fromValue(fileItem);
    }

    return QFileSystemModel::data(index, role);    
}

QModelIndex FileSystemModel::setRootPathOwn(const QString &newPath)
{
    QTimer::singleShot(0, this, SLOT(deleteUnusedFileItems()));

    QModelIndex index = setRootPath(newPath);

    FileManager::self()->iconsViewController()->selectItem(index);

    return index;
}

void FileSystemModel::deleteUnusedFileItems()
{
    static QList<FileItem *> toBeDeleted;

    while (!toBeDeleted.isEmpty())
    {
        FileItem *fileItem = toBeDeleted.takeFirst();
        QMap<QString, FileItem *>::iterator it = fileItems.find(fileItem->fileName());
        if (it != fileItems.end())
        {
            qDebug() << fileItem->fileName();
            fileItems.erase(it);
            delete fileItem;
        }
    }

    toBeDeleted = fileItems.values();
}
