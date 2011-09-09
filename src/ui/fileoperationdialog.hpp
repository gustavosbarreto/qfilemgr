#ifndef _FILEOPERATIONDIALOG_HPP
#define _FILEOPERATIONDIALOG_HPP

#include "fileoperation.hpp"
#include "ui_fileoperationdialog.h"

class FileOperation;

class FileOperationDialog: public QDialog
{
    Q_OBJECT

public:
    FileOperationDialog(FileOperation *fileOperation, QWidget *parent = 0);

    void setText(const QString &text);

public slots:
    virtual void reject();

private slots:
    void updateProgressBar(int value);
    void notifyExitStatus(FileOperation::ExitStatus status);

private:
    Ui::FileOperationDialog ui;
    FileOperation *m_fileOperation;
};

#endif
