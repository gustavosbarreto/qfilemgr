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

#ifndef _COPYDIRECTORYTHREAD_HPP
#define _COPYDIRECTORYTHREAD_HPP

#include <QList>

#include "fileoperation.hpp"

class QDir;
class DirectorySizeCalculator;
class CopyFileThread;

class CopyDirectoryThread: public FileOperation
{
    Q_OBJECT

public:
    CopyDirectoryThread(const QString &source, const QString &destination, QObject *parent);
    ~CopyDirectoryThread();

public slots:
    virtual void stop();

protected:
    virtual void run();

private:
    bool copyDirectory(const QDir &sourceDir, const QDir &destDir);

private:
    QString m_source;
    QString m_destination;
    quint64 m_copiedFiles;
    quint64 m_totalFiles;
    DirectorySizeCalculator *m_calculator;
    CopyFileThread *m_copyFileThread;
};

#endif
