#include "locationarrow.hpp"

#include <QMouseEvent>
#include <QMenu>

LocationArrow::LocationArrow(QWidget *parent):
    QLabel(parent)
{
    setToolTip(trUtf8("Click here to open the menu"));
}

void LocationArrow::mousePressEvent(QMouseEvent *e)
{
    QMenu menu(this);
    menu.addAction(trUtf8("Go to parent folder"), this, SIGNAL(goToParentFolderActivated()));
    menu.exec(mapToGlobal(e->pos()));
}
