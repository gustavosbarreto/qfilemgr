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

#include "directorysizecalculator.hpp"
#include "fileitem.hpp"

#include <QDir>

DirectorySizeCalculator::DirectorySizeCalculator(const QString &path, QObject *parent):
    QThread(parent),
    m_fileName(path),
    m_abort(false),
    m_totalSize(0),
    m_totalFiles(0)
{
}

DirectorySizeCalculator::DirectorySizeCalculator(FileItem *fileItem, QObject *parent):
    QThread(parent),
    m_fileName(fileItem->fileName()),
    m_abort(false),
    m_totalSize(0),
    m_totalFiles(0)
{
}

quint64 DirectorySizeCalculator::totalSize() const
{
    return m_totalSize;
}

quint64 DirectorySizeCalculator::totalFiles() const
{
    return m_totalFiles;
}

void DirectorySizeCalculator::stop()
{
    m_abort = true;
}

void DirectorySizeCalculator::run()
{
    m_totalSize = 0;
    m_totalFiles = 0;

    processNextDirectory(QDir(m_fileName));
}

void DirectorySizeCalculator::processNextDirectory(const QDir &dir)
{
    if (m_abort)
        return;

    static int counter = 0;

    foreach (QFileInfo info, dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot))
    {
        if (!info.isSymLink())
        {
            if (info.isDir())
                processNextDirectory(QDir(info.filePath()));
            else
            {
                m_totalSize += info.size();
                m_totalFiles++;
                if (counter++ >= 1000)
                {
                    emit totalSizeUpdated(m_totalSize);
                    counter = 0;
                }
            }
        }
    }
}
