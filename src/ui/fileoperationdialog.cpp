#include "fileoperationdialog.hpp"
#include "fileoperation.hpp"

#include <QMessageBox>

FileOperationDialog::FileOperationDialog(FileOperation *fileOperation,
                                         QWidget *parent):
    QDialog(parent),
    m_fileOperation(fileOperation)
{
    ui.setupUi(this);

    connect(fileOperation, SIGNAL(progress(int)), SLOT(updateProgressBar(int)));
    connect(fileOperation, SIGNAL(exited(FileOperation::ExitStatus)), SLOT(notifyExitStatus(FileOperation::ExitStatus)));
    fileOperation->start();
}

void FileOperationDialog::setText(const QString &text)
{
    ui.label->setText(text);
}

void FileOperationDialog::reject()
{
    m_fileOperation->stop();
    QDialog::reject();
}

void FileOperationDialog::updateProgressBar(int value)
{
    if (ui.progressBar->maximum() == 0)
        ui.progressBar->setMaximum(100);

    ui.progressBar->setValue(value);
}

void FileOperationDialog::notifyExitStatus(FileOperation::ExitStatus status)
{
    if (status == FileOperation::Success)
        accept();
    if (status == FileOperation::Error)
    {
        QMessageBox::critical(this, trUtf8("Error"), trUtf8("An error occurred in this operation"));
        reject();
    }
}

