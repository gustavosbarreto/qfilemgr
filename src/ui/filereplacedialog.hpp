#ifndef _FILEREPLACEDIALOG_HPP
#define _FILEREPLACEDIALOG_HPP

#include "ui_filereplacedialog.h"

class FileReplaceDialog: public QDialog
{
public:
    FileReplaceDialog(QWidget *parent = 0);

private:
    Ui::FileReplaceDialog ui;
};

#endif
