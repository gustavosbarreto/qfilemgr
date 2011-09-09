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

#include <QApplication>
#include <QFile>
#include <QSettings>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

#include "filemanager.hpp"
#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Aplica o style
    QSettings settings(QSettings::SystemScope, "ossystems", "appearance");
    QFile file(QString("%1/../share/efieme/styles/%2.qss")
               .arg(app.applicationDirPath())
               .arg(settings.value("Style/Name").toString()));
    file.open(QFile::ReadOnly);
    app.setStyleSheet(file.readAll());

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                       QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QTranslator t;
    t.load(QString("%1/../share/ostd-fm/i18n/%2")
          .arg(QCoreApplication::applicationDirPath())
          .arg(QLocale::system().name()));
    app.installTranslator(&t);

    FileManager::self()->mainWindow()->show();

    return app.exec();
}
