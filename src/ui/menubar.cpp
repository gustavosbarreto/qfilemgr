#include "menubar.hpp"

#include <QMenu>

MenuBar::MenuBar(QWidget *parent):
    QMenuBar(parent)
{
    QMenu *fileMenu = new QMenu("File", this);
    QMenu *editMenu = new QMenu("Edit", this);
    QMenu *goMenu = new QMenu("Go", this);
    addMenu(fileMenu);
    addMenu(editMenu);
    addMenu(goMenu);
}
