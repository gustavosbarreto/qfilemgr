/*
 *  Copyright (C) 2011 O.S. Systems Softwares Ltda. All rights reserved.
 *  Authors:
 *    Luis Gustavo S. Barreto <gustavosbarreto@gmail.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

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

