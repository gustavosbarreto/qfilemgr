#ifndef _REMOVEFILETHREAD_HPP
#define _REMOVEFILETHREAD_HPP

#include <QList>

#include "fileoperation.hpp"

class QDir;
class DirectorySizeCalculator;

class RemoveFileThread: public FileOperation
{
    Q_OBJECT

public:
    RemoveFileThread(const QString &fileName, QObject *parent);
    ~RemoveFileThread();

public slots:
    virtual void stop();

protected:
    virtual void run();

private:
    bool removeDirectory(const QDir &dir);

private:
    QString m_fileName;
    quint64 m_removedFiles;
    quint64 m_totalFiles;
    DirectorySizeCalculator *m_calculator;
};

#endif
