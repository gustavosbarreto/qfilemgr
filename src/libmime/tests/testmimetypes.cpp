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

#include <QtTest>

#include "mimedetector.hpp"

class TestMimeTypes: public QObject
{
    Q_OBJECT

private slots:
    void testMimeType()
    {
        MimeDetector d("/usr/share/mime/mime.cache");

        QCOMPARE(d.mimeTypesForFileName("text.txt").at(0).mimeType(), QString("text/plain"));
        QCOMPARE(d.mimeTypesForFileName("text.tar").at(0).mimeType(), QString("application/x-tar"));
        QCOMPARE(d.mimeTypesForFileName("text.tar.gz").at(0).mimeType(), QString("application/x-compressed-tar"));
        QCOMPARE(d.mimeTypesForFileName("text.tar.bz2").at(0).mimeType(), QString("application/x-bzip-compressed-tar"));
        QCOMPARE(d.mimeTypesForFileName("text.gz").at(0).mimeType(), QString("application/x-gzip"));
        QCOMPARE(d.mimeTypesForFileName("text.bz2").at(0).mimeType(), QString("application/x-bzip"));
        QCOMPARE(d.mimeTypesForFileName("text.jpg").at(0).mimeType(), QString("image/jpeg"));
        QCOMPARE(d.mimeTypesForFileName("text.gif").at(0).mimeType(), QString("image/gif"));
        QCOMPARE(d.mimeTypesForFileName("text.png").at(0).mimeType(), QString("image/png"));
        QCOMPARE(d.mimeTypesForFileName("README.in").at(0).mimeType(), QString("text/x-readme"));
        QCOMPARE(d.mimeTypesForFileName("core").at(0).mimeType(), QString("application/x-core"));
        QCOMPARE(d.mimeTypesForFileName("libtest.so.5").at(0).mimeType(), QString("application/x-sharedlib"));
    }

    void testMediaType()
    {
        MimeDetector d("/usr/share/mime/mime.cache");

        QCOMPARE(d.mimeTypesForFileName("text.txt").at(0).mediaType(), QString("text"));
        QCOMPARE(d.mimeTypesForFileName("text.tar").at(0).mediaType(), QString("application"));
        QCOMPARE(d.mimeTypesForFileName("text.tar.gz").at(0).mediaType(), QString("application"));
        QCOMPARE(d.mimeTypesForFileName("text.tar.bz2").at(0).mediaType(), QString("application"));
        QCOMPARE(d.mimeTypesForFileName("text.gz").at(0).mediaType(), QString("application"));
        QCOMPARE(d.mimeTypesForFileName("text.bz2").at(0).mediaType(), QString("application"));
        QCOMPARE(d.mimeTypesForFileName("text.jpg").at(0).mediaType(), QString("image"));
        QCOMPARE(d.mimeTypesForFileName("text.gif").at(0).mediaType(), QString("image"));
        QCOMPARE(d.mimeTypesForFileName("text.png").at(0).mediaType(), QString("image"));
        QCOMPARE(d.mimeTypesForFileName("README.in").at(0).mediaType(), QString("text"));
        QCOMPARE(d.mimeTypesForFileName("core").at(0).mediaType(), QString("application"));
        QCOMPARE(d.mimeTypesForFileName("libtest.so.5").at(0).mediaType(), QString("application"));
    }

    void testSubType()
    {
        MimeDetector d("/usr/share/mime/mime.cache");

        QCOMPARE(d.mimeTypesForFileName("text.txt").at(0).subType(), QString("plain"));
        QCOMPARE(d.mimeTypesForFileName("text.tar").at(0).subType(), QString("x-tar"));
        QCOMPARE(d.mimeTypesForFileName("text.tar.gz").at(0).subType(), QString("x-compressed-tar"));
        QCOMPARE(d.mimeTypesForFileName("text.tar.bz2").at(0).subType(), QString("x-bzip-compressed-tar"));
        QCOMPARE(d.mimeTypesForFileName("text.gz").at(0).subType(), QString("x-gzip"));
        QCOMPARE(d.mimeTypesForFileName("text.bz2").at(0).subType(), QString("x-bzip"));
        QCOMPARE(d.mimeTypesForFileName("text.jpg").at(0).subType(), QString("jpeg"));
        QCOMPARE(d.mimeTypesForFileName("text.gif").at(0).subType(), QString("gif"));
        QCOMPARE(d.mimeTypesForFileName("text.png").at(0).subType(), QString("png"));
        QCOMPARE(d.mimeTypesForFileName("README.in").at(0).subType(), QString("x-readme"));
        QCOMPARE(d.mimeTypesForFileName("core").at(0).subType(), QString("x-core"));
        QCOMPARE(d.mimeTypesForFileName("libtest.so.5").at(0).subType(), QString("x-sharedlib"));
    }
};

QTEST_MAIN(TestMimeTypes)

#include "testmimetypes.moc"
