#ifndef _FILEMANAGER_HPP
#define _FILEMANAGER_HPP

#include <QObject>
#include <QMap>
#include <QStringList>
#include <QSize>

class MainWindow;
class FileSystemModel;
class PlacesModel;
class IconsViewController;
class HistoryList;
class MimeCache;
class IconEngine;
class DeviceManager;
class QModelIndex;

class FileManager: public QObject
{
    Q_OBJECT

public:
    FileManager();

    MainWindow *mainWindow() const;
    FileSystemModel *fileSystemModel() const;
    PlacesModel *placesModel() const;
    IconsViewController *iconsViewController() const;
    HistoryList *historyList() const;
    MimeCache *mimeCache() const;
    IconEngine *iconEngine() const;

    QString pasteFile(const QString &source, const QString &destination, bool cut = false);
    bool removeFile(const QString &fileName);
    QString newFolder(const QString &workingDirectory);
    QString renameFile(const QString &fileName);

    static FileManager *self();

private slots:
    void emitRootPathChanged(const QString &path);

signals:
    void rootPathChanged(const QModelIndex &);
};

#endif
