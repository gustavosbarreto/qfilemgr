#ifndef _CUSTOMPROGRESSBAR_HPP
#define _CUSTOMPROGRESSBAR_HPP

#include <QProgressBar>

class CustomProgressBar: public QProgressBar
{
public:
    CustomProgressBar(QWidget *parent = 0);

    virtual QString text() const;
};

#endif
