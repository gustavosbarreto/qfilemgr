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

/*
    WARNING: this code is duplicated by osthindesktop-desk project.
    Therefore, any changes you make in this file must be applied in it's duplicata.
*/

#ifndef _STARTUPNOTIFYHANDLER_HPP
#define _STARTUPNOTIFYHANDLER_HPP

#include <QString>
#include <QObject>
#include <QList>

#define TIMEOUT 20000 // timeout in ms

class QTimer;


class StartupNotifyHandler: public QObject
{
    Q_OBJECT

public:
    StartupNotifyHandler(QObject *parent = 0);

    void add(const QString &id, void *data = NULL);
    void remove(const QString &id);
    bool contains(const QString &id);
    void *getUserData(const QString &id);
    void print();

private:
    struct StartupData
    {
        QString id;
        QTimer *timer;
        void *userData;

        StartupData(const QString &_id, QTimer *t, void *data = NULL);
    };

    QList<StartupData> m_startup;

private slots:
    void onTimeout();
};

#endif
