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
