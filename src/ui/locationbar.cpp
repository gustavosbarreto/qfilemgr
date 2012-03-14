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

#include "locationbar.hpp"
#include "locationbaritem.hpp"
#include "filemanager.hpp"
#include "filesystemmodel.hpp"
#include "placesmodel.hpp"
#include "locationarrow.hpp"

#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QUrl>
#include <QModelIndex>
#include <QFileSystemModel>
#include <QCoreApplication>
#include <QTimer>
#include <QFontMetrics>
#include <QScrollBar>
#include <QLinkedList>

LocationBar::LocationBar(QWidget *parent):
    QScrollArea(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QSizePolicy sp(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sp.setHorizontalStretch(0);
    sp.setVerticalStretch(0);
    sp.setHeightForWidth(sizePolicy().hasHeightForWidth());
    setSizePolicy(sp);
    setWidgetResizable(true);

    QFrame *w = new QFrame(this);
    w->setStyleSheet("QFrame { background-color: transparent; }");
    w->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::MinimumExpanding);

    QHBoxLayout *l = new QHBoxLayout(w);
    l->setContentsMargins(0, 0, 0, 0);

    setWidget(w);

    m_icon = new QLabel(this);
    m_icon->setPixmap(QPixmap(":/images/ostd-fm.png").scaled(16, 16));
    l->addWidget(m_icon);

    connect(FileManager::self(), SIGNAL(rootPathChanged(const QModelIndex &)),
            SLOT(updateItems(const QModelIndex &)));
}

void LocationBar::resizeEvent(QResizeEvent *event)
{
    QTimer::singleShot(0, this, SLOT(updateButtonsVisibility()));
    QScrollArea::resizeEvent(event);
}

void LocationBar::updateButtons(int startIndex)
{
    bool createButton;
    const int oldButtonCount = m_buttons.count();
    const QString path = FileManager::self()->fileSystemModel()->rootPath();

    int idx = startIndex;
    bool hasNext = true;
    do
    {
        createButton = (idx - startIndex >= oldButtonCount);
        const bool isFirstButton = (idx == startIndex);
        const QString dirName = path.section('/', idx, idx);
        hasNext = !dirName.isEmpty();
        if (hasNext)
        {
            QString text = dirName;

            if (isFirstButton)
            {
                QString currentPlacePath = FileManager::self()->placesModel()->currentPlacePath();
                text = FileManager::self()->placesModel()->index(currentPlacePath).data(Qt::DisplayRole).toString();
            }

            LocationBarItem *item = NULL;
            if (createButton)
            {
                item = new LocationBarItem;
                item->uri = path;
                item->button = new QPushButton(text, widget());
                item->button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
                item->button->setFocusPolicy(Qt::NoFocus);
                item->button->setToolTip(trUtf8("Go to this location"));
                connect(item->button, SIGNAL(clicked()), item, SLOT(goToUri()));

                ((QBoxLayout*)widget()->layout())->insertWidget(widget()->layout()->count(), item->button);

                if (!isFirstButton)
                {
                    item->arrow = new LocationArrow(widget());
                    item->arrow->setPixmap(QPixmap(":/images/miniarrow.png"));
                    connect(item->arrow, SIGNAL(goToParentFolderActivated()), item, SLOT(goToParentFolder()));
                    ((QBoxLayout*)widget()->layout())->insertWidget(widget()->layout()->count() - 1, item->arrow);
                }

                setFixedHeight(widget()->sizeHint().height() + 4);

                m_buttons.append(item);
            }
            else
            {
                item = m_buttons[idx - startIndex];
                item->button->setText(text);
            }

            if (isFirstButton)
                item->uri = FileManager::self()->placesModel()->currentPlacePath();

            ++idx;

        }
    } while (hasNext);

    const int newButtonCount = idx - startIndex;
    if (newButtonCount < oldButtonCount) {
        const QList<LocationBarItem *>::iterator begin = m_buttons.begin() + newButtonCount;
        const QList<LocationBarItem *>::iterator end = m_buttons.end();
        QList<LocationBarItem *>::iterator it = begin;
        while (it != end) {
            (*it)->button->hide();
            (*it)->button->deleteLater();

            if ((*it)->arrow)
            {
                (*it)->arrow->hide();
                (*it)->arrow->deleteLater();
            }

            delete (*it);
            ++it;
        }
        m_buttons.erase(begin, end);
    }

    updateButtonsVisibility();
}

void LocationBar::updateItems(const QModelIndex &index)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QString path = index.data(QFileSystemModel::FilePathRole).toString();

    QString currentPlacePath = FileManager::self()->placesModel()->currentPlacePath();
    int startIndex = currentPlacePath.count('/');
    QIcon icon = qvariant_cast<QIcon>(FileManager::self()->placesModel()->index(currentPlacePath).data(Qt::DecorationRole));

    setUpdatesEnabled(false); // Desabilita paint event

    // Modificacoes na interface grafica
    m_icon->setPixmap(icon.pixmap(16, 16));
    updateButtons(startIndex);

    QTimer::singleShot(0, this, SLOT(enablePaintEvent())); // Habilita paint event No proximo event loop
}

void LocationBar::updateButtonsVisibility()
{
    const int buttonsCount = m_buttons.count();
    if (buttonsCount == 0)
        return;

    int availableWidth = frameRect().width();
    availableWidth -= m_icon->size().width();

    int requiredButtonWidth = 0;
    foreach (const LocationBarItem* item, m_buttons)
        requiredButtonWidth += item->button->minimumWidth();

    QList<LocationBarItem *>::const_iterator it = m_buttons.constEnd();
    const QList<LocationBarItem *>::const_iterator begin = m_buttons.constBegin();
    bool isLastButton = true;
    bool hasHiddenButtons = false;

    QLinkedList<LocationBarItem *> buttonsToShow;
    while (it != begin) {
        --it;
        LocationBarItem *item = (*it);
        availableWidth -= item->button->width() + widget()->layout()->spacing();
        if (item->arrow)
            availableWidth -= item->arrow->width() + widget()->layout()->spacing();
        if ((availableWidth <= 0) && !isLastButton) {
            item->button->hide();
            if (item->arrow)
                item->arrow->hide();
            hasHiddenButtons = true;
        }
        else
            buttonsToShow.append(item);

        isLastButton = false;
    }

    foreach (LocationBarItem *item, buttonsToShow)
    {
        item->button->show();
        if (item->arrow)
            item->arrow->show();
    }
}

void LocationBar::enablePaintEvent()
{
    setUpdatesEnabled(true);
}
