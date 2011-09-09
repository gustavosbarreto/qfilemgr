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

#include "copyfilethread.hpp"

#include <QFile>

CopyFileThread::CopyFileThread(const QString &source, const QString &destination, QObject *parent):
    FileOperation(parent),
    m_source(source),
    m_destination(destination)
{
}

CopyFileThread::CopyFileThread(QObject *parent):
    FileOperation(parent)
{
}

void CopyFileThread::setSource(const QString &source)
{
    m_source = source;
}

void CopyFileThread::setDestination(const QString &destination)
{
    m_destination = destination;
}

void CopyFileThread::run()
{
    QFile sourceFile(m_source);
    if (!sourceFile.open(QFile::ReadOnly))
    {
        setExitStatus(Error);
        return;
    }

    QFile destinationFile(m_destination);
    if (!destinationFile.open(QFile::WriteOnly))
    {
        setExitStatus(Error);
        return;
    }

    quint64 total = 0;

    while (!sourceFile.atEnd())
    {
        if (isToStop())
        {
            setExitStatus(Stopped);

            sourceFile.close();
            destinationFile.close();
            QFile::remove(m_destination);

            return;
        }

        QByteArray data = sourceFile.read(4096);
        total += data.size();

        emit progress((100 * total) / sourceFile.size());

        destinationFile.write(data);
    }

    setExitStatus(Success);
}
