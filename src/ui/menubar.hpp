#ifndef _MENUBAR_HPP
#define _MENUBAR_HPP

#include <QMenuBar>

class MenuBar: public QMenuBar
{
    Q_OBJECT

public:
    MenuBar(QWidget *parent);
};

#endif
