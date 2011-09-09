#ifndef _FILEPROPERTIESPAGE_HPP
#define _FILEPROPERTIESPAGE_HPP

#include <QWidget>

class FilePropertiesPage: public QWidget
{
public:
    FilePropertiesPage(QWidget *parent): QWidget(parent) { }

    virtual void save() = 0;
};

#endif
