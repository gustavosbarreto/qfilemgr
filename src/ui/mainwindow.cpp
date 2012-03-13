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

#include "mainwindow.hpp"
#include "filemanager.hpp"
#include "locationbar.hpp"
#include "filesystemmodel.hpp"
#include "menubar.hpp"
#include "iconsviewcontroller.hpp"
#include "historylist.hpp"
#include "placesmodel.hpp"

#include <QPushButton>
#include <QDir>
#include <QDebug>
#include <QUrl>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QX11Info>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    m_notified(false)
{
    ui.setupUi(this);

    setContextMenuPolicy(Qt::NoContextMenu);

    ui.actionBack->setIcon(QIcon::fromTheme("go-previous", QIcon(":/images/go-previous.png")));
    ui.actionForward->setIcon(QIcon::fromTheme("go-next", QIcon(":/images/go-next.png")));

    connect(ui.actionBack, SIGNAL(triggered()), SLOT(goBack()));
    connect(ui.actionForward, SIGNAL(triggered()), SLOT(goForward()));

    connect(FileManager::self(), SIGNAL(rootPathChanged(const QModelIndex &)),
            ui.iconView, SLOT(setRootIndex(const QModelIndex &)));
    connect(FileManager::self(), SIGNAL(rootPathChanged(const QModelIndex &)),
            SLOT(changeCurrentUri(const QModelIndex &)));

    ui.toolBar->addWidget(new LocationBar(ui.toolBar));

    ui.iconView->setRootIndex(FileManager::self()->fileSystemModel()->setRootPathOwn(QDir::homePath()));

    FileManager::self()->iconsViewController()->selectItem(ui.iconView->rootIndex());

    QAction *cutAction = new QAction(this);
    cutAction->setShortcuts(QKeySequence::Cut);
    connect(cutAction, SIGNAL(triggered()), FileManager::self()->iconsViewController(), SLOT(cutFile()));
    addAction(cutAction);

    QAction *copyAction = new QAction(this);
    copyAction->setShortcuts(QKeySequence::Copy);
    connect(copyAction, SIGNAL(triggered()), FileManager::self()->iconsViewController(), SLOT(copyFile()));
    addAction(copyAction);

    QAction *pasteAction = new QAction(this);
    pasteAction->setShortcuts(QKeySequence::Paste);
    connect(pasteAction, SIGNAL(triggered()), FileManager::self()->iconsViewController(), SLOT(pasteFile()));
    addAction(pasteAction);
}

void MainWindow::changeCurrentUri(const QModelIndex &index)
{
    FileManager::self()->historyList()->append(index.data(QFileSystemModel::FilePathRole).toString());

    ui.actionBack->setEnabled(FileManager::self()->historyList()->canBack());
    ui.actionForward->setEnabled(FileManager::self()->historyList()->canForward());

    QString name = QFileInfo(index.data(QFileSystemModel::FilePathRole).toString()).fileName();
    QModelIndex placeIndex = FileManager::self()->placesModel()->index(index.data(QFileSystemModel::FilePathRole).toString());
    if (placeIndex.isValid())
        name = placeIndex.data(Qt::DisplayRole).toString();

    setWindowTitle(name);
}

void MainWindow::goBack()
{
    QString uri = FileManager::self()->historyList()->goBack();
    FileManager::self()->fileSystemModel()->setRootPathOwn(uri);
}

void MainWindow::goForward()
{
    QString uri = FileManager::self()->historyList()->goForward();
    FileManager::self()->fileSystemModel()->setRootPathOwn(uri);
}
