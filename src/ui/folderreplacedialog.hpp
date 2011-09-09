#ifndef _FOLDERREPLACEDIALOG_HPP
#define _FOLDERREPLACEDIALOG_HPP

#include "ui_folderreplacedialog.h"

class FolderReplaceDialog: public QDialog
{
public:
    FolderReplaceDialog(QWidget *parent = 0);

private:
    Ui::FolderReplaceDialog ui;
};

#endif
