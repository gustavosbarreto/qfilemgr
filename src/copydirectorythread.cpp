#include "copydirectorythread.hpp"
#include "copyfilethread.hpp"
#include "directorysizecalculator.hpp"
#include "filemanager.hpp"
#include "mainwindow.hpp"

#include <QFile>
#include <QDir>
#include <QDebug>

CopyDirectoryThread::CopyDirectoryThread(const QString &source, const QString &destination, QObject *parent):
    FileOperation(parent),
    m_source(source),
    m_destination(destination),
    m_calculator(NULL),
    m_copyFileThread(NULL)
{
    m_calculator = new DirectorySizeCalculator(m_source, this);
    m_copyFileThread = new CopyFileThread(this);
}

CopyDirectoryThread::~CopyDirectoryThread()
{
    stop();

    if (m_calculator)
        m_calculator->deleteLater();
    if (m_copyFileThread)
        m_copyFileThread->deleteLater();
}

void CopyDirectoryThread::stop()
{
    if (m_calculator)
    {
        m_calculator->stop();
        m_calculator->wait();
    }

    if (m_copyFileThread)
    {
        m_copyFileThread->stop();
        m_copyFileThread->wait();
    }

    FileOperation::stop();
}

void CopyDirectoryThread::run()
{
    m_calculator->start();
    m_calculator->wait();

    m_copiedFiles = 0;
    m_totalFiles = m_calculator->totalFiles();

    m_calculator->deleteLater();
    m_calculator = NULL;

    if (!copyDirectory(m_source, m_destination))
    {
        if (exitStatus() != Stopped)
            setExitStatus(Error);

        return;
    }

    setExitStatus(Success);
}

bool CopyDirectoryThread::copyDirectory(const QDir &sourceDir, const QDir &destDir)
{
    if (isToStop())
    {
        setExitStatus(Stopped);
        return false;
    }

    if (!destDir.exists())
    {
        QDir d = destDir;
        if (!d.cdUp())
            return false;
        if (!d.mkdir(destDir.dirName()))
            return false;
    }

    foreach (const QFileInfo &file, sourceDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot))
    {
        if (file.isDir())
        {
            if (!copyDirectory(QDir(file.filePath()), QDir(destDir.path() + "/" + file.fileName())))
                return false;
        }
        else
        {
            emit progress((100 * m_copiedFiles) / m_totalFiles);

            m_copyFileThread->setSource(file.filePath());
            m_copyFileThread->setDestination(destDir.path() + "/" + file.fileName());
            m_copyFileThread->start();
            m_copyFileThread->wait();

            if (m_copyFileThread->exitStatus() != Success)
            {
                setExitStatus(m_copyFileThread->exitStatus());
                return false;
            }
            else
                m_copiedFiles++;
        }
    }

    return true;
}
