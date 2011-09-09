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

#include "detailsframe.hpp"
#include "filemanager.hpp"
#include "mimedetector.hpp"
#include "iconsviewcontroller.hpp"
#include "filesystemmodel.hpp"
#include "placesmodel.hpp"

#include <QFileSystemModel>
#include <QModelIndex>
#include <QUrl>
#include <QDebug>

DetailsFrame::DetailsFrame(QWidget *widget):
    QFrame(widget)
{
    ui.setupUi(this);

    setFrameShape(QFrame::NoFrame);

    connect(FileManager::self()->iconsViewController(),
            SIGNAL(itemSelected(const QModelIndex &)), SLOT(updateInfo(const QModelIndex &)));
}

void DetailsFrame::updateInfo(const QModelIndex &index)
{
    QString path = index.data(QFileSystemModel::FilePathRole).toString();
    QString name = QFileInfo(path).fileName();
    QModelIndex placeIndex = FileManager::self()->placesModel()->index(path);
    if (placeIndex.isValid())
        name = placeIndex.data(Qt::DisplayRole).toString();

    QFontMetrics metrics(font());
    if (name.length() > 80)
        name = metrics.elidedText(name, Qt::ElideRight, metrics.averageCharWidth() * 80);

    QPixmap icon = QPixmap(index.data(FileSystemModel::MimeTypeIconRole).toString());
    if (placeIndex.isValid())
        icon = qvariant_cast<QIcon>(placeIndex.data(Qt::DecorationRole)).pixmap(48, 48);

    ui.icon->setPixmap(icon);
    ui.name->setText(name);
    ui.type->setText(index.data(FileSystemModel::MimeTypeDescriptionRole).toString());
}
