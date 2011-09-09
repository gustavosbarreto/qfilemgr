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

#include "removefilethread.hpp"
#include "directorysizecalculator.hpp"
#include "filemanager.hpp"
#include "mainwindow.hpp"

#include <QFile>
#include <QDir>
#include <QDebug>

RemoveFileThread::RemoveFileThread(const QString &fileName, QObject *parent):
    FileOperation(parent),
    m_fileName(fileName),
    m_calculator(NULL)
{
    m_calculator = new DirectorySizeCalculator(fileName, this);
}

RemoveFileThread::~RemoveFileThread()
{
    stop();

    if (m_calculator)
        m_calculator->deleteLater();
}

void RemoveFileThread::stop()
{
    if (m_calculator)
    {
        m_calculator->stop();
        m_calculator->wait();
    }

    FileOperation::stop();
}

void RemoveFileThread::run()
{
    if (QFileInfo(m_fileName).isDir())
    {
        m_calculator->start();
        m_calculator->wait();

        m_removedFiles = 0;
        m_totalFiles = m_calculator->totalFiles();

        m_calculator->deleteLater();
        m_calculator = NULL;

        if (!removeDirectory(QDir(m_fileName)))
        {
            if (exitStatus() != Stopped)
                setExitStatus(Error);
            return;
        }
    }
    else
    {
        if (!QFile::remove(m_fileName))
        {
            setExitStatus(Error);
            return;
        }
    }

    setExitStatus(Success);
}

bool RemoveFileThread::removeDirectory(const QDir &dir)
{
    if (isToStop())
    {
        setExitStatus(Stopped);
        return false;
    }

    QDir d = dir;

    foreach (const QFileInfo &file, dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot))
    {
        if (file.isDir())
        {
            if (!removeDirectory(QDir(file.filePath())))
                return false;
        }
        else
        {
            emit progress((100 * m_removedFiles) / m_totalFiles);

            m_removedFiles++;

            if (!d.remove(file.fileName()))
                return false;
        }
    }

    QString name = d.dirName();
    if (!d.cdUp())
        return false;
    d.rmdir(name);

    return true;
}
