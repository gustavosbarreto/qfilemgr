#include "directorysizecalculator.hpp"
#include "fileitem.hpp"

#include <QDir>

DirectorySizeCalculator::DirectorySizeCalculator(const QString &path, QObject *parent):
    QThread(parent),
    m_fileName(path),
    m_abort(false),
    m_totalSize(0),
    m_totalFiles(0)
{
}

DirectorySizeCalculator::DirectorySizeCalculator(FileItem *fileItem, QObject *parent):
    QThread(parent),
    m_fileName(fileItem->fileName()),
    m_abort(false),
    m_totalSize(0),
    m_totalFiles(0)
{
}

quint64 DirectorySizeCalculator::totalSize() const
{
    return m_totalSize;
}

quint64 DirectorySizeCalculator::totalFiles() const
{
    return m_totalFiles;
}

void DirectorySizeCalculator::stop()
{
    m_abort = true;
}

void DirectorySizeCalculator::run()
{
    m_totalSize = 0;
    m_totalFiles = 0;

    processNextDirectory(QDir(m_fileName));
}

void DirectorySizeCalculator::processNextDirectory(const QDir &dir)
{
    if (m_abort)
        return;

    static int counter = 0;

    foreach (QFileInfo info, dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot))
    {
        if (!info.isSymLink())
        {
            if (info.isDir())
                processNextDirectory(QDir(info.filePath()));
            else
            {
                m_totalSize += info.size();
                m_totalFiles++;
                if (counter++ >= 1000)
                {
                    emit totalSizeUpdated(m_totalSize);
                    counter = 0;
                }
            }
        }
    }
}
