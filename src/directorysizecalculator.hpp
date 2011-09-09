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

#ifndef _DIRECTORYSIZECALCULATOR_HPP
#define _DIRECTORYSIZECALCULATOR_HPP

#include <QThread>

class QDir;

class FileItem;

class DirectorySizeCalculator: public QThread
{
    Q_OBJECT

public:
    DirectorySizeCalculator(const QString &path, QObject *parent = 0);
    DirectorySizeCalculator(FileItem *fileItem, QObject *parent = 0);

    quint64 totalSize() const;
    quint64 totalFiles() const;

public slots:
    void stop();

protected:
    virtual void run();

private:
    void processNextDirectory(const QDir &dir);

private:
    QString m_fileName;
    bool m_abort;
    quint64 m_totalSize;
    quint64 m_totalFiles;

signals:
    void totalSizeUpdated(quint64);
};

#endif
