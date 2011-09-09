CONFIG += link_pkgconfig qdbus
#PKGCONFIG += OSGuiWidgets desktopentry libstartup-notification-1.0 cuterpcclient

DEFINES += SN_API_NOT_YET_FROZEN

TARGET = ostd-fm

RESOURCES += ../resources.qrc

TRANSLATIONS = ../i18n/pt_BR.ts \
               ../i18n/es_ES.ts \
               ../i18n/ja_JP.ts

translations.name = Translations ${QMAKE_FILE_BASE}
translations.CONFIG += no_link target_predeps
translations.commands = @$$[QT_INSTALL_BINS]/lrelease ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_OUT} &> /dev/null
translations.input = TRANSLATIONS
translations.output = ${QMAKE_FILE_BASE}.qm
translations.clean = ${QMAKE_FILE_BASE}.qm
QMAKE_EXTRA_COMPILERS += translations

include(ui/ui.pri)
include(libmime/src/src.pri)

HEADERS += \
  filemanager.hpp \
  filesystemmodel.hpp \
  iconsviewcontroller.hpp \
  fileitem.hpp \
  iconengine.hpp \
  processlauncher.hpp \
  startupnotifyhandler.hpp \
  directorysizecalculator.hpp \
  volumefreespaceinfo.hpp \
  mountpoint.hpp \
  fileoperation.hpp \
  copyfilethread.hpp \
  copydirectorythread.hpp \
  removefilethread.hpp \
  historylist.hpp \
  placesmodel.hpp \
  trash.hpp \
  trashinfo.hpp

SOURCES += \
  main.cpp \
  filemanager.cpp \
  filesystemmodel.cpp \
  iconsviewcontroller.cpp \
  fileitem.cpp \
  iconengine.cpp \
  processlauncher.cpp \
  startupnotifyhandler.cpp \
  directorysizecalculator.cpp \
  volumefreespaceinfo.cpp \
  mountpoint.cpp \
  fileoperation.cpp \
  copyfilethread.cpp \
  copydirectorythread.cpp \
  removefilethread.cpp \
  historylist.cpp \
  placesmodel.cpp \
  trash.cpp \
  trashinfo.cpp

target.path = $$PREFIX/bin

qm.files = *.qm
qm.path = $$PREFIX/share/ostd-fm/i18n
qm.CONFIG += no_check_exist

INSTALLS += target qm
