#include "fileoperation.hpp"

FileOperation::FileOperation(QObject *parent):
    QThread(parent),
    m_exitStatus(NoStatus),
    m_stop(false)
{
    qRegisterMetaType<FileOperation::ExitStatus>("FileOperation::ExitStatus");
}

FileOperation::ExitStatus FileOperation::exitStatus() const
{
    return m_exitStatus;
}

void FileOperation::stop()
{
    m_stop = true;
}

bool FileOperation::isToStop() const
{
    return m_stop;
}

void FileOperation::setExitStatus(FileOperation::ExitStatus status)
{
    m_exitStatus = status;

    emit exited(status);
}
