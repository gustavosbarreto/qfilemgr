#ifndef _LOCATIONARROW_HPP
#define _LOCATIONARROW_HPP

#include <QLabel>

class LocationArrow: public QLabel
{
    Q_OBJECT

public:
    LocationArrow(QWidget *parent = 0);

protected:
    virtual void mousePressEvent(QMouseEvent *e);

signals:
    void goToParentFolderActivated();
};

#endif
