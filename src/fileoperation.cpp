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

#include "fileoperation.hpp"

FileOperation::FileOperation(QObject *parent):
    QThread(parent),
    m_exitStatus(NoStatus),
    m_stop(false)
{
    qRegisterMetaType<FileOperation::ExitStatus>("FileOperation::ExitStatus");
}

FileOperation::ExitStatus FileOperation::exitStatus() const
{
    return m_exitStatus;
}

void FileOperation::stop()
{
    m_stop = true;
}

bool FileOperation::isToStop() const
{
    return m_stop;
}

void FileOperation::setExitStatus(FileOperation::ExitStatus status)
{
    m_exitStatus = status;

    emit exited(status);
}
