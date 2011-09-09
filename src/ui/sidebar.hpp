#ifndef _SIDEBAR_HPP
#define _SIDEBAR_HPP

#include <QListView>

class HalDevice;

class SideBar: public QListView
{
    Q_OBJECT

public:
    SideBar(QWidget *parent);

protected:
    virtual void mousePressEvent(QMouseEvent *e);

private:
    void goToPlacePath();

private slots:
    void release();
    void selectItem(const QString &path);
};

#endif
