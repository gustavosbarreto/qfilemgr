#include "customprogressbar.hpp"

CustomProgressBar::CustomProgressBar(QWidget *parent):
    QProgressBar(parent)
{
}

QString CustomProgressBar::text() const
{
    if (maximum() == 0 && value() == 0)
        return trUtf8("Processing files");
    else
        return QProgressBar::text();
}
