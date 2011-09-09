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

#include "iconsview.hpp"
#include "filemanager.hpp"
#include "filesystemmodel.hpp"
#include "iconsviewcontroller.hpp"
#include "filepropertiesdialog.hpp"

#include <QMouseEvent>
#include <QMenu>
#include <QDebug>

IconsView::IconsView(QWidget *parent):
    QListView(parent)
{
    setIconSize(QSize(48, 48));
    setViewMode(QListView::IconMode);
    setLayoutDirection(Qt::LeftToRight);
    setResizeMode(QListView::Adjust);
    setMovement(QListView::Static);
    setModel(FileManager::self()->fileSystemModel());

    FileManager::self()->iconsViewController()->setSelectionModel(selectionModel());

    connect(this, SIGNAL(doubleClicked(const QModelIndex &)),
            FileManager::self()->iconsViewController(),
            SLOT(triggerItem(const QModelIndex &)));

    connect(FileManager::self()->iconsViewController(),
            SIGNAL(itemSelected(const QModelIndex &)),
            SLOT(selectItem(const QModelIndex &)));

    updateGridSize();
}

void IconsView::resizeEvent(QResizeEvent* event)
{
    QListView::resizeEvent(event);
    updateGridSize();
}

void IconsView::mousePressEvent(QMouseEvent *e)
{
    QListView::mousePressEvent(e);

    FileSystemModel *model = FileManager::self()->fileSystemModel();

    const QModelIndex index = indexAt(e->pos());
    if (!index.isValid())
    {
        clearSelection();

        QModelIndex current = model->index(model->rootPath());

        FileManager::self()->iconsViewController()->selectItem(current);

        if (e->button() & Qt::RightButton)
            FileManager::self()->iconsViewController()->itemContextMenu(current, mapToGlobal(e->pos()));
    }
    else if (e->button() & Qt::RightButton)
    {
        if (!selectionModel()->isSelected(index))
            clearSelection();
        else
            FileManager::self()->iconsViewController()->selectItem(index);

        FileManager::self()->iconsViewController()->itemContextMenu(index, mapToGlobal(e->pos()));
    }
    else if (e->button() & Qt::LeftButton)
        FileManager::self()->iconsViewController()->selectItem(index);
}

void IconsView::updateGridSize()
{
    static int itemWidth = 120;
    static int itemHeight = 80;
    static int size = 48;
    static int spacing = 6;

    setIconSize(QSize(size, size));
    setGridSize(QSize(itemWidth + spacing * 2, itemHeight + spacing));
}

void IconsView::selectItem(const QModelIndex &index)
{
    selectionModel()->select(index, QItemSelectionModel::SelectCurrent);
    scrollTo(index);
}
