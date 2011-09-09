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

#ifndef _PLACESMODEL_HPP
#define _PLACESMODEL_HPP

#include <QAbstractItemModel>

class HalDevice;
class Place;

class PlacesModel: public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Roles
    {
        PathRole = Qt::UserRole + 1,
        TypeRole,
        DataRole
    };

    enum Type
    {
        LocalDirectoryType,
        VolumeType
    };

    PlacesModel(QObject *parent = 0);

    QString currentPlacePath() const;

    QModelIndex index(const QString &path) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private slots:
    void updateCurrentPlacePath(const QModelIndex &index);
//    void deviceMounted(HalDevice *device);
//    void deviceUnmounted(HalDevice *device);

private:
    QList<Place *> m_places;
    QString m_currentPlacePath;

signals:
    void currentPlacePathChanged(const QString &);
};

#endif
