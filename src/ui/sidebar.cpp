#include "sidebar.hpp"
#include "filemanager.hpp"
#include "filesystemmodel.hpp"
#include "mainwindow.hpp"
#include "placesmodel.hpp"

#include <QListWidgetItem>
#include <QDir>
#include <QContextMenuEvent>
#include <QMenu>
#include <QMessageBox>
#include <QDebug>

SideBar::SideBar(QWidget *parent):
    QListView(parent)
{
    setIconSize(QSize(32, 32));
    setFocusPolicy(Qt::NoFocus);
    setSelectionRectVisible(false);

    setModel(FileManager::self()->placesModel());

    connect(FileManager::self()->placesModel(), SIGNAL(currentPlacePathChanged(const QString &)),
            SLOT(selectItem(const QString &)));
}

void SideBar::mousePressEvent(QMouseEvent *e)
{
    QListView::mousePressEvent(e);

    const QModelIndex index = indexAt(e->pos());
    if (!index.isValid())
        return;

    PlacesModel::Type type = (PlacesModel::Type)index.data(PlacesModel::TypeRole).toInt();

    if (e->button() & Qt::RightButton && type == PlacesModel::VolumeType)
    {
        QMenu menu(FileManager::self()->mainWindow());
        menu.addAction(trUtf8("Release"), this, SLOT(release()));
        menu.exec(mapToGlobal(e->pos()));
    }
    else if (e->button() & Qt::LeftButton)
        goToPlacePath();
}

void SideBar::goToPlacePath()
{
    QModelIndex current = selectionModel()->selectedIndexes().at(0);
    FileManager::self()->fileSystemModel()->setRootPathOwn(current.data(PlacesModel::PathRole).toString());
}

void SideBar::release()
{
    QModelIndex current = selectionModel()->selectedIndexes().at(0);
}

void SideBar::selectItem(const QString &path)
{
    QModelIndex index = FileManager::self()->placesModel()->index(path);
    if (!selectionModel()->selectedIndexes().isEmpty() && selectionModel()->selectedIndexes().at(0) == index)
        return;

    selectionModel()->select(FileManager::self()->placesModel()->index(path), QItemSelectionModel::ClearAndSelect);
}
