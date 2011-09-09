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

#ifndef _GENERALFILEPROPERTIESPAGE_HPP
#define _GENERALFILEPROPERTIESPAGE_HPP

#include "filepropertiespage.hpp"

#include "ui_generalfilepropertiespage.h"

class QModelIndex;
class DirectorySizeCalculator;

class GeneralFilePropertiesPage: public FilePropertiesPage
{
    Q_OBJECT

public:
    GeneralFilePropertiesPage(const QModelIndex &index, QWidget *parent);
    ~GeneralFilePropertiesPage();

    virtual void save();

private:
    const QString fileSizeToString(quint64 bytes);

private slots:
    void directorySizeCalculated();
    void updateSize(quint64 size);

private:
    Ui::GeneralFilePropertiesPage ui;
    DirectorySizeCalculator *m_directorySizeCalculator;
};

#endif

