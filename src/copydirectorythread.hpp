#ifndef _COPYDIRECTORYTHREAD_HPP
#define _COPYDIRECTORYTHREAD_HPP

#include <QList>

#include "fileoperation.hpp"

class QDir;
class DirectorySizeCalculator;
class CopyFileThread;

class CopyDirectoryThread: public FileOperation
{
    Q_OBJECT

public:
    CopyDirectoryThread(const QString &source, const QString &destination, QObject *parent);
    ~CopyDirectoryThread();

public slots:
    virtual void stop();

protected:
    virtual void run();

private:
    bool copyDirectory(const QDir &sourceDir, const QDir &destDir);

private:
    QString m_source;
    QString m_destination;
    quint64 m_copiedFiles;
    quint64 m_totalFiles;
    DirectorySizeCalculator *m_calculator;
    CopyFileThread *m_copyFileThread;
};

#endif
