#include "iconsviewcontroller.hpp"
#include "filemanager.hpp"
#include "filesystemmodel.hpp"
#include "mimedetector.hpp"
#include "mainwindow.hpp"
#include "filepropertiesdialog.hpp"
#include "processlauncher.hpp"

#include <QModelIndex>
#include <QProcess>
#include <QDebug>
#include <QMenu>
#include <QMimeData>
#include <QClipboard>
#include <QUrl>

//#include <desktopentry.hpp>

IconsViewController::IconsViewController(QObject *parent):
    QObject(parent),
    m_discardSelection(false)
{
}

void IconsViewController::setSelectionModel(QItemSelectionModel *model)
{
    m_selectionModel = model;

    connect(model, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            SLOT(selectionChanged(const QItemSelection &, const QItemSelection &)));
}

void IconsViewController::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    if (!selected.indexes().isEmpty() && m_discardSelection)
    {
        m_selectionModel->select(selected.indexes().at(0), QItemSelectionModel::Deselect | QItemSelectionModel::Clear);
        m_discardSelection = false;
    }
    else if (!selected.indexes().isEmpty())
        itemSelected(selected.indexes().at(0));
}

void IconsViewController::itemContextMenu(const QModelIndex &index, const QPoint &point)
{
    bool overCurrentIndex = (model()->rootPath() == (index.data(QFileSystemModel::FilePathRole).toString()));

    QMenu menu(FileManager::self()->mainWindow());

    if (overCurrentIndex)
    {
        menu.addAction(trUtf8("New Folder"), this, SLOT(newFolder()));
        menu.addSeparator();
        menu.addAction(trUtf8("Paste"), this, SLOT(pasteFile()));
        menu.addSeparator();
        menu.addAction(trUtf8("Properties"), this, SLOT(openPropertiesDialog()));
    }
    else
    {
        menu.addAction(trUtf8("Open"), this, SLOT(openFile()));
        menu.addSeparator();
        menu.addAction(trUtf8("Cut"), this, SLOT(cutFile()));
        menu.addAction(trUtf8("Copy"), this, SLOT(copyFile()));
        menu.addAction(trUtf8("Paste"), this, SLOT(pasteFile()));
        menu.addSeparator();
        menu.addAction(trUtf8("Remove"), this, SLOT(removeFile()));
        menu.addAction(trUtf8("Rename"), this, SLOT(renameFile()));
        menu.addSeparator();
        menu.addAction(trUtf8("Properties"), this, SLOT(openPropertiesDialog()));
    }

    menu.exec(point);
}

void IconsViewController::triggerItem(const QModelIndex &index)
{
    QString filePath = index.data(QFileSystemModel::FilePathRole).toString();
    if (QFileInfo(filePath).isDir())
        model()->setRootPathOwn(filePath);
    else
    {
        #if 0
        QSettings s(index.data(FileSystemModel::DefaultApplicationDesktopFileRole).toString(), OSDesktopEntryFormat);
        s.beginGroup("Desktop Entry");
        QString exec = s.value("Exec").toString();
        exec = exec.left(exec.indexOf("\%") - 1);

        QStringList args;
        args << filePath;

        ProcessLauncher::startDetached(exec, args);
        #endif
    }
}

void IconsViewController::selectItem(const QModelIndex &index)
{
    if (!m_selectionModel->selectedIndexes().isEmpty())
    {
        QModelIndex current = m_selectionModel->selectedIndexes().at(0);
        m_selectionModel->select(current, QItemSelectionModel::Deselect | QItemSelectionModel::Clear);
    }

    emit itemSelected(index);
}

FileSystemModel *IconsViewController::model() const
{
    return FileManager::self()->fileSystemModel();
}

void IconsViewController::discardNextSelection()
{
    m_discardSelection = true;
}

void IconsViewController::acceptNextSelection()
{
    m_discardSelection = false;
}

void IconsViewController::newFolder()
{
    QString fileName = FileManager::self()->newFolder(model()->rootPath());
    if (!fileName.isEmpty())
        selectItem(model()->index(fileName));
}

void IconsViewController::renameFile()
{
    QModelIndex current = m_selectionModel->selectedIndexes().at(0);

    QString fileName = FileManager::self()->renameFile(current.data(QFileSystemModel::FilePathRole).toString());
    if (!fileName.isEmpty())
    {
    }
}

void IconsViewController::removeFile()
{
    QModelIndex current = m_selectionModel->selectedIndexes().at(0);

    QString fileName = current.data(QFileSystemModel::FilePathRole).toString();

    selectItem(model()->index(model()->rootPath()));
    discardNextSelection();

    if (!FileManager::self()->removeFile(fileName))
    {
        acceptNextSelection();
        selectItem(model()->index(fileName));
    }
}

void IconsViewController::openFile()
{
    QModelIndex current = m_selectionModel->selectedIndexes().at(0);
    triggerItem(current);
}

void IconsViewController::cutFile()
{
    QMimeData *mimeData = model()->mimeData(m_selectionModel->selectedIndexes());
    mimeData->setData("application/x-oss-cutaction", QByteArray("1"));
    QApplication::clipboard()->setMimeData(mimeData);
}

void IconsViewController::copyFile()
{
    QMimeData *mimeData = model()->mimeData(m_selectionModel->selectedIndexes());
    QApplication::clipboard()->setMimeData(mimeData);
}

void IconsViewController::pasteFile()
{
    bool cut;
    QByteArray cutData = QApplication::clipboard()->mimeData()->data("application/x-oss-cutaction");
    cut = !cutData.isEmpty(); // Se é uma operacao de recortar

    QByteArray uriData = QApplication::clipboard()->mimeData()->data("text/uri-list");
    if (uriData.isEmpty())
        return;

    QString source = QUrl(QString(uriData).trimmed()).path();
    QString destination = model()->index(model()->rootPath()).data(QFileSystemModel::FilePathRole).toString();

    QString pasted = FileManager::self()->pasteFile(source, destination, cut);
    if (!pasted.isEmpty())
        selectItem(model()->index(pasted));
}

void IconsViewController::openPropertiesDialog()
{
    QModelIndex current = m_selectionModel->selectedIndexes().at(0);

    FilePropertiesDialog dialog(current, FileManager::self()->mainWindow());
    dialog.exec();
}
