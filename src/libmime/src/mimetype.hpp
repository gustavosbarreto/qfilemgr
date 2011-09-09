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

#ifndef _MIMETYPE_HPP
#define _MIMETYPE_HPP

#include <QString>
#include <QList>
#include <QDebug>

#include "mimeaction.hpp"

class MimeType
{
public:
    MimeType();
    MimeType(const QString mimeType);
    MimeType(const MimeType &other);

    const QString mimeType() const;
    const QString mediaType() const;
    const QString subType() const;
    const QString description() const;
    const QString icon() const;
    QList<MimeAction> actions() const;

    bool isSubclass(const QString &subclass) const;

private:
    bool isSuperType() const;

private:
    QString m_mimeType;
    QString m_mediaType;
    QString m_subType;
};

inline QDebug operator<<(QDebug dbg, const MimeType &mime)
{
    dbg.nospace() << "MimeType(mimeType=" << mime.mimeType() << ", mediaType=" << mime.mediaType() << ", subType=" << mime.subType() << ")";
    return dbg.space();
}

#endif
