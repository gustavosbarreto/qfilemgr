#include "copyfilethread.hpp"

#include <QFile>

CopyFileThread::CopyFileThread(const QString &source, const QString &destination, QObject *parent):
    FileOperation(parent),
    m_source(source),
    m_destination(destination)
{
}

CopyFileThread::CopyFileThread(QObject *parent):
    FileOperation(parent)
{
}

void CopyFileThread::setSource(const QString &source)
{
    m_source = source;
}

void CopyFileThread::setDestination(const QString &destination)
{
    m_destination = destination;
}

void CopyFileThread::run()
{
    QFile sourceFile(m_source);
    if (!sourceFile.open(QFile::ReadOnly))
    {
        setExitStatus(Error);
        return;
    }

    QFile destinationFile(m_destination);
    if (!destinationFile.open(QFile::WriteOnly))
    {
        setExitStatus(Error);
        return;
    }

    quint64 total = 0;

    while (!sourceFile.atEnd())
    {
        if (isToStop())
        {
            setExitStatus(Stopped);

            sourceFile.close();
            destinationFile.close();
            QFile::remove(m_destination);

            return;
        }

        QByteArray data = sourceFile.read(4096);
        total += data.size();

        emit progress((100 * total) / sourceFile.size());

        destinationFile.write(data);
    }

    setExitStatus(Success);
}
