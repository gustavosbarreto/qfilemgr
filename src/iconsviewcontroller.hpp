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

#ifndef _ICONSVIEWCONTROLLER_HPP
#define _ICONSVIEWCONTROLLER_HPP

#include <QObject>

class QModelIndex;
class QFileInfo;
class QPoint;
class QItemSelectionModel;
class FileSystemModel;
class QItemSelection;

class IconsViewController: public QObject
{
    Q_OBJECT

public:
    IconsViewController(QObject *parent = 0);

    void setSelectionModel(QItemSelectionModel *model);

    void itemContextMenu(const QModelIndex &index, const QPoint &point);

public slots:
    void triggerItem(const QModelIndex &index);
    void selectItem(const QModelIndex &index);
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void openFile();
    void cutFile();
    void copyFile();
    void pasteFile();

private:
    FileSystemModel *model() const;

    void discardNextSelection();
    void acceptNextSelection();

private slots:
    void newFolder();
    void renameFile();
    void removeFile();
    void openPropertiesDialog();

signals:
    void itemTriggered(const QFileInfo &file);
    void itemSelected(const QModelIndex &index);

private:
    QItemSelectionModel *m_selectionModel;
    bool m_discardSelection;
};

#endif
