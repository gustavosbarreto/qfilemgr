#ifndef _ICONSVIEW_HPP
#define _ICONSVIEW_HPP

#include <QListView>

class IconsView: public QListView
{
    Q_OBJECT

public:
    IconsView(QWidget *parent);

protected:
    virtual void resizeEvent(QResizeEvent* event);
    virtual void mousePressEvent(QMouseEvent *e);

private:
    void updateGridSize();

private slots:
    void selectItem(const QModelIndex &index);
};

#endif
