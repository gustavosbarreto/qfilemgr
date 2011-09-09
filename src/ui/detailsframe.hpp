#ifndef _DETAILSFRAME_HPP
#define _DETAILSFRAME_HPP

#include "ui_detailsframe.h"

class QModelIndex;

class DetailsFrame: public QFrame
{
    Q_OBJECT

public:
    DetailsFrame(QWidget *parent);

public slots:
    void updateInfo(const QModelIndex &index);

private:
    Ui::DetailsFrame ui;
};

#endif
