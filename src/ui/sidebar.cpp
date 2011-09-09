/*
 *  Copyright (C) 2011 O.S. Systems Softwares Ltda. All rights reserved.
 *  Authors:
 *    Luis Gustavo S. Barreto <gustavosbarreto@gmail.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

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
