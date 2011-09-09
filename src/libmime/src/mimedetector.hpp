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

#ifndef _MIMEDETECTOR_HPP
#define _MIMEDETECTOR_HPP

#include <QMap>
#include <QStringList>

#include "weightedmimetype.hpp"

class QDataStream;
class MimeType;

class MimeDetector
{
public:
    MimeDetector(const QString mimeCacheFile);
    ~MimeDetector();

    MimeType *mimeTypeForFileName(const QString &fileName);

private:
    QStringList lookupMimeTypesForGlobFileName(const QString &fileName);
    QStringList lookupGlobLiteral(const QString &fileName);
    QStringList lookupGlobSuffix(QString fileName, bool ignoreCase, int len);
    QStringList lookupGlobFileNameMatch(const QString &fileName);
    QStringList lookupGlobNodeSuffix(const QString &fileName,
                                                 int numEntries, int offset,
                                                 bool ignoreCase, int len, QString pattern = QString());

    void initMimeTypes();
    void addMimeType(const MimeType &mimeType);

    QString getString(int offset, bool regExp = false);
    
    quint32 aliasListOffset();
    quint32 aliasesCount(int offset);
    quint32 literalListOffset();
    quint32 literalListCount(int offset);
    quint32 reverseSuffixTreeOffset();
    quint32 reverseSuffixTreeCount(int offset);
    quint32 globListOffset();
    quint32 globListCount(int offset);
    quint32 iconListOffset();
    quint32 iconListCount(int offset);

private:
    QDataStream *stream;
    QMap<QString, QStringList> m_mimeTypes;
    QMap<QString, MimeType *> m_mimeTypess;
};

#endif
