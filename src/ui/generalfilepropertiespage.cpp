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

#include "generalfilepropertiespage.hpp"
#include "filemanager.hpp"
#include "filesystemmodel.hpp"
#include "fileitem.hpp"
#include "directorysizecalculator.hpp"
#include "volumefreespaceinfo.hpp"
#include "mountpoint.hpp"
#include "placesmodel.hpp"

#include <QDebug>

static const quint64 kb = 1024;
static const quint64 mb = 1024 * kb;
static const quint64 gb = 1024 * mb;
static const quint64 tb = 1024 * gb;

GeneralFilePropertiesPage::GeneralFilePropertiesPage(const QModelIndex &index, QWidget *parent):
    FilePropertiesPage(parent),
    m_directorySizeCalculator(NULL)
{
    ui.setupUi(this);

    FileSystemModel *model = FileManager::self()->fileSystemModel();
    FileItem *fileItem = qvariant_cast<FileItem *>(index.data(FileSystemModel::FileItemRole));

    QIcon icon = qvariant_cast<QIcon>(model->data(index, Qt::DecorationRole));
    ui.fileIcon->setPixmap(icon.pixmap(48, 48));

    ui.fileName->setText(QFileInfo(fileItem->fileName()).fileName());

    QModelIndex placeIndex = FileManager::self()->placesModel()->index(fileItem->fileName());
    if (placeIndex.isValid())
    {
        ui.fileName->setText(placeIndex.data(Qt::DisplayRole).toString());
        ui.fileName->setReadOnly(true);
        ui.fileIcon->setPixmap(qvariant_cast<QIcon>(placeIndex.data(Qt::DecorationRole)).pixmap(48, 48));
        ui.location->hide();
        ui.locationLabel->hide();
    }

    ui.fileType->setText(model->data(index, FileSystemModel::MimeTypeDescriptionRole).toString());
    ui.location->setText(QFontMetrics(font()).elidedText(fileItem->filePath(), Qt::ElideMiddle, width()));
    ui.location->setToolTip(fileItem->filePath());

    if (fileItem->isDir())
    {
        ui.size->setText(trUtf8("Calculating..."));

        m_directorySizeCalculator = new DirectorySizeCalculator(fileItem, this);
        connect(m_directorySizeCalculator, SIGNAL(finished()), SLOT(directorySizeCalculated()));
        connect(m_directorySizeCalculator, SIGNAL(totalSizeUpdated(quint64)), SLOT(updateSize(quint64)));
        m_directorySizeCalculator->start();

        VolumeFreeSpaceInfo info = VolumeFreeSpaceInfo::freeSpaceInfo(MountPoint::mountPointForFilePath(fileItem->fileName()));
        ui.capacityBar->setValue((100 - (int)(100.0 * quint64(info.available() / kb) / quint64(info.size() / kb))));

        // Esconde widgets especificos para arquivo
        ui.applicationName->hide();
        ui.applicationIcon->hide();
        ui.openWithsLabel->hide();
    }
    else
    {
        ui.applicationName->setText(model->data(index, FileSystemModel::DefaultApplicationNameRole).toString());
        ui.applicationIcon->setPixmap(QPixmap(model->data(index, FileSystemModel::DefaultApplicationIconRole).toString())
                                      .scaled(22, 22, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        ui.size->setText(fileSizeToString(fileItem->size()));

        // Esconde widgets especificos para diretorio
        ui.lastLine->hide();
        ui.capacityLabel->hide();
        ui.capacityBar->hide();
    }
}

GeneralFilePropertiesPage::~GeneralFilePropertiesPage()
{
    if (m_directorySizeCalculator)
    {
        m_directorySizeCalculator->stop();
        m_directorySizeCalculator->wait();
        m_directorySizeCalculator->deleteLater();
    }
}

void GeneralFilePropertiesPage::save()
{
}

const QString GeneralFilePropertiesPage::fileSizeToString(quint64 bytes)
{
    if (bytes >= tb)
        return QString("%1 TB").arg(QLocale().toString(qreal(bytes) / tb, 'f', 3));
    if (bytes >= gb)
        return QString("%1 GB").arg(QLocale().toString(qreal(bytes) / gb, 'f', 2));
    if (bytes >= mb)
        return QString("%1 MB").arg(QLocale().toString(qreal(bytes) / mb, 'f', 1));
    if (bytes >= kb)
        return QString("%1 KB").arg(QLocale().toString(bytes / kb));

    return QString("%1 %2").arg(QLocale().toString(bytes)).arg(bytes > 1 ? "bytes" : "byte");
}

void GeneralFilePropertiesPage::directorySizeCalculated()
{
    ui.size->setText(fileSizeToString(m_directorySizeCalculator->totalSize()));
}

void GeneralFilePropertiesPage::updateSize(quint64 size)
{
    ui.size->setText(trUtf8("Calculating... %1").arg(fileSizeToString(size)));
}
