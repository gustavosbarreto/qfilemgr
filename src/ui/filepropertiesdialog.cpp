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

#include "filepropertiesdialog.hpp"
#include "generalfilepropertiespage.hpp"
//#include "sharefilepropertiespage.hpp"

#include <QSettings>

FilePropertiesDialog::FilePropertiesDialog(const QModelIndex &index, QWidget *parent):
    QDialog(parent)
{
    ui.setupUi(this);
    ui.tabWidget->addTab(new GeneralFilePropertiesPage(index, this), trUtf8("General"));

    QSettings s(QSettings::SystemScope, "ossystems", "sharing", this);
//    if (s.value("General/Usershares").toBool())
//        ui.tabWidget->addTab(new ShareFilePropertiesPage(index, this), trUtf8("Sharing"));
}

int FilePropertiesDialog::exec()
{
    int result = QDialog::exec();
    if (!result)
        return result;
    return result;
}
