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

#ifndef _FILEOPERATION_HPP
#define _FILEOPERATION_HPP

#include <QThread>
#include <QMetaType>

class FileOperation: public QThread
{
    Q_OBJECT

public:
    enum ExitStatus
    {
        NoStatus,
        Stopped,
        Success,
        Error
    };

    FileOperation(QObject *parent = 0);

    ExitStatus exitStatus() const;

public slots:
    virtual void stop();

protected:
    bool isToStop() const;
    void setExitStatus(ExitStatus status);

private:
    ExitStatus m_exitStatus;
    bool m_stop;

signals:
    void progress(int);
    void exited(FileOperation::ExitStatus);
};

Q_DECLARE_METATYPE(FileOperation::ExitStatus)

#endif
