#ifndef _TRASH_HPP
#define _TRASH_HPP

#include <QObject>

class Trash: public QObject
{
public:
    Trash(QObject *parent = 0);

    static QString trashPath();
};

#endif
