#include "filemanager.hpp"
#include "filesystemmodel.hpp"
#include "iconsviewcontroller.hpp"
#include "mimecache.hpp"
#include "iconengine.hpp"
#include "mainwindow.hpp"
#include "copyfilethread.hpp"
#include "copydirectorythread.hpp"
#include "removefilethread.hpp"
#include "fileoperationdialog.hpp"
#include "historylist.hpp"
#include "placesmodel.hpp"

#include <QUrl>
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>

Q_GLOBAL_STATIC(FileManager, fileManager)

FileManager::FileManager():
    QObject()
{
}

FileSystemModel *FileManager::fileSystemModel() const
{
    static FileSystemModel *model = NULL;
    if (!model)
    {
        model = new FileSystemModel;
        connect(model, SIGNAL(rootPathChanged(const QString &)), SLOT(emitRootPathChanged(const QString &)));
    }
    return model;
}

PlacesModel *FileManager::placesModel() const
{
    static PlacesModel *model = NULL;
    if (!model)
        model = new PlacesModel;
    return model;
}

MainWindow *FileManager::mainWindow() const
{
    static MainWindow *window = NULL;
    if (!window)
        window = new MainWindow;
    return window;
}

IconsViewController *FileManager::iconsViewController() const
{
    static IconsViewController *controller = NULL;
    if (!controller)
        controller = new IconsViewController;
    return controller;
}

HistoryList *FileManager::historyList() const
{
    static HistoryList *list = NULL;
    if (!list)
        list = new HistoryList;
    return list;
}

MimeCache *FileManager::mimeCache() const
{
    static MimeCache *cache = NULL;
    if (!cache)
        cache = new MimeCache("/usr/share/mime/mime.cache");
    return cache;
}

IconEngine *FileManager::iconEngine() const
{
    static IconEngine *engine = NULL;
    if (!engine)
        engine = new IconEngine("oxygen", 48);
    return engine;
}

QString FileManager::pasteFile(const QString &source, const QString &destination, bool cut)
{
    QString newName;

    bool hasError;

    if (!cut) // Operacao de copia
    {
        if (!QFileInfo(source).isDir())
        {
            newName = destination + "/" + QFileInfo(source).fileName();

            CopyFileThread *thread = new CopyFileThread(source, newName, this);
            FileOperationDialog d(thread, FileManager::self()->mainWindow());
            d.setWindowTitle(trUtf8("Copying file"));
            d.setText(trUtf8("Copying '%1' to '%2'").arg(QFileInfo(source).fileName()).arg(destination));
            d.exec();

            hasError = (thread->exitStatus() == FileOperation::Error);

            thread->wait();
            thread->deleteLater();
        }
        else
        {
            newName = destination + "/" + QDir(source).dirName();

            CopyDirectoryThread *thread = new CopyDirectoryThread(source, newName, mainWindow());
            FileOperationDialog d(thread, FileManager::self()->mainWindow());
            d.setWindowTitle(trUtf8("Copying file"));
            d.setText(trUtf8("Copying '%1' to '%2'").arg(QFileInfo(source).fileName()).arg(destination));
            d.exec();

            hasError = (thread->exitStatus() == FileOperation::Error);

            thread->wait();
            thread->deleteLater();
        }

        if (hasError)
        {
            QMessageBox::critical(FileManager::self()->mainWindow(),
                                  trUtf8("Error"),
                                  trUtf8("Unable to paste the file"));
        }
    }
    else // Operacao de recorte
    {
        newName = QFileInfo(destination).filePath() + "/" + QFileInfo(source).fileName();
        rename(source.toUtf8().data(), newName.toUtf8().data());
    }

    if (!hasError)
        return newName;
    else
        return QString();
}

bool FileManager::removeFile(const QString &fileName)
{
    QMessageBox::StandardButton r = QMessageBox::question(FileManager::self()->mainWindow(),
                                                          trUtf8("Confirm"),
                                                          trUtf8("Remove the file?"),
                                                          QMessageBox::Yes | QMessageBox::No);

    if (r == QMessageBox::No)
        return false;

    RemoveFileThread *thread = new RemoveFileThread(fileName, mainWindow());
    FileOperationDialog d(thread, mainWindow());
    d.setWindowTitle(trUtf8("Remove file"));
    d.setText(trUtf8("Removing '%1'").arg(QFileInfo(fileName).fileName()));
    d.exec();

    bool hasError = (thread->exitStatus() == FileOperation::Error);

    if (hasError)
    {
        QMessageBox::critical(FileManager::self()->mainWindow(),
                              trUtf8("Error"),
                              trUtf8("Unable to remove the file"));
    }

    return !hasError;
}

QString FileManager::newFolder(const QString &workingDirectory)
{
    bool ok;
    QString folderName = QInputDialog::getText(FileManager::self()->mainWindow(),
                                               trUtf8("New Folder"),
                                               trUtf8("Folder name:"),
                                               QLineEdit::Normal,
                                               "",
                                               &ok);

    if (!ok)
        return QString();

    QDir dir(workingDirectory);
    if (!dir.mkdir(folderName))
    {
        QMessageBox::critical(FileManager::self()->mainWindow(),
                              trUtf8("Error"),
                              trUtf8("Unable to create the folder"));

        return QString();
    }

    return dir.absoluteFilePath(folderName);
}

QString FileManager::renameFile(const QString &fileName)
{
    bool ok;
    QString newName = QInputDialog::getText(FileManager::self()->mainWindow(),
                                            trUtf8("Rename file"),
                                            trUtf8("Rename the file '%1' to:").arg(QFileInfo(fileName).fileName()),
                                            QLineEdit::Normal,
                                            QFileInfo(fileName).fileName(),
                                            &ok);

    if (!ok)
        return QString();

    bool hasError;

    QDir dir(QFileInfo(fileName).absolutePath());

    if (!hasError)
        hasError = newName.contains('/');
    if (!hasError)
        hasError = !dir.rename(QFileInfo(fileName).fileName(), newName);
    if (hasError)
    {
        QMessageBox::critical(FileManager::self()->mainWindow(),
                              trUtf8("Error"),
                              trUtf8("Unable to rename the file"));
        return QString();
    }

    return dir.absoluteFilePath(newName);
}

FileManager *FileManager::self()
{
    return fileManager();
}

void FileManager::emitRootPathChanged(const QString &path)
{
    emit rootPathChanged(fileSystemModel()->index(path));
}
