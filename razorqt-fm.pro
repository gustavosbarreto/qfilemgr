TEMPLATE = subdirs
SUBDIRS += src

style.path = $$PREFIX/share/efieme/styles/
style.files = styles/*.qss

pixmaps.files = images/ostd-fm.png
pixmaps.path = $$PREFIX/share/pixmaps/

INSTALLS += style pixmaps
