#ifndef _LOCATIONBARITEM_HPP
#define _LOCATIONBARITEM_HPP

#include <QObject>

class QPushButton;
class QLabel;

class LocationBarItem: public QObject
{
    Q_OBJECT

public:
    LocationBarItem();

    QString uri;
    QPushButton *button;
    QLabel *arrow;

public slots:
    void goToUri();
    void goToParentFolder();
};

#endif
