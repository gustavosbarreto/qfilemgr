#ifndef _LOCATIONBAR_HPP
#define _LOCATIONBAR_HPP

#include <QScrollArea>
#include <QMap>
#include <QStringList>
#include <QLabel>

class QPushButton;
class LocationBarItem;
class QModelIndex;

class LocationBar: public QScrollArea
{
    Q_OBJECT

public:
    LocationBar(QWidget *parent = 0);

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    void updateButtons(int startIndex);

private slots:
    void updateItems(const QModelIndex &index);
    void updateButtonsVisibility();
    void enablePaintEvent();

private:
    QList<LocationBarItem *> m_buttons;
    QLabel *m_icon;
};

#endif
