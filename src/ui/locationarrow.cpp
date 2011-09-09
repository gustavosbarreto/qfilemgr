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

#include "locationarrow.hpp"

#include <QMouseEvent>
#include <QMenu>

LocationArrow::LocationArrow(QWidget *parent):
    QLabel(parent)
{
    setToolTip(trUtf8("Click here to open the menu"));
}

void LocationArrow::mousePressEvent(QMouseEvent *e)
{
    QMenu menu(this);
    menu.addAction(trUtf8("Go to parent folder"), this, SIGNAL(goToParentFolderActivated()));
    menu.exec(mapToGlobal(e->pos()));
}
