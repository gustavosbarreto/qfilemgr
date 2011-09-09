#ifndef _FILEPROPERTIESDIALOG_HPP
#define _FILEPROPERTIESDIALOG_HPP

#include "ui_filepropertiesdialog.h"

class QModelIndex;

class FilePropertiesDialog: public QDialog
{
    Q_OBJECT

public:
    FilePropertiesDialog(const QModelIndex &index, QWidget *parent = 0);

public slots:
    virtual int exec();

private:
    Ui::FilePropertiesDialog ui;
};

#endif
