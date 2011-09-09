#include "removefilethread.hpp"
#include "directorysizecalculator.hpp"
#include "filemanager.hpp"
#include "mainwindow.hpp"

#include <QFile>
#include <QDir>
#include <QDebug>

RemoveFileThread::RemoveFileThread(const QString &fileName, QObject *parent):
    FileOperation(parent),
    m_fileName(fileName),
    m_calculator(NULL)
{
    m_calculator = new DirectorySizeCalculator(fileName, this);
}

RemoveFileThread::~RemoveFileThread()
{
    stop();

    if (m_calculator)
        m_calculator->deleteLater();
}

void RemoveFileThread::stop()
{
    if (m_calculator)
    {
        m_calculator->stop();
        m_calculator->wait();
    }

    FileOperation::stop();
}

void RemoveFileThread::run()
{
    if (QFileInfo(m_fileName).isDir())
    {
        m_calculator->start();
        m_calculator->wait();

        m_removedFiles = 0;
        m_totalFiles = m_calculator->totalFiles();

        m_calculator->deleteLater();
        m_calculator = NULL;

        if (!removeDirectory(QDir(m_fileName)))
        {
            if (exitStatus() != Stopped)
                setExitStatus(Error);
            return;
        }
    }
    else
    {
        if (!QFile::remove(m_fileName))
        {
            setExitStatus(Error);
            return;
        }
    }

    setExitStatus(Success);
}

bool RemoveFileThread::removeDirectory(const QDir &dir)
{
    if (isToStop())
    {
        setExitStatus(Stopped);
        return false;
    }

    QDir d = dir;

    foreach (const QFileInfo &file, dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot))
    {
        if (file.isDir())
        {
            if (!removeDirectory(QDir(file.filePath())))
                return false;
        }
        else
        {
            emit progress((100 * m_removedFiles) / m_totalFiles);

            m_removedFiles++;

            if (!d.remove(file.fileName()))
                return false;
        }
    }

    QString name = d.dirName();
    if (!d.cdUp())
        return false;
    d.rmdir(name);

    return true;
}
