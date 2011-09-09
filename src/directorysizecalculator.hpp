#ifndef _DIRECTORYSIZECALCULATOR_HPP
#define _DIRECTORYSIZECALCULATOR_HPP

#include <QThread>

class QDir;

class FileItem;

class DirectorySizeCalculator: public QThread
{
    Q_OBJECT

public:
    DirectorySizeCalculator(const QString &path, QObject *parent = 0);
    DirectorySizeCalculator(FileItem *fileItem, QObject *parent = 0);

    quint64 totalSize() const;
    quint64 totalFiles() const;

public slots:
    void stop();

protected:
    virtual void run();

private:
    void processNextDirectory(const QDir &dir);

private:
    QString m_fileName;
    bool m_abort;
    quint64 m_totalSize;
    quint64 m_totalFiles;

signals:
    void totalSizeUpdated(quint64);
};

#endif
