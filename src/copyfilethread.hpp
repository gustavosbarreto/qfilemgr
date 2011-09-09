#ifndef _COPYFILETHREAD_HPP
#define _COPYFILETHREAD_HPP

#include "fileoperation.hpp"

class CopyFileThread: public FileOperation
{
    Q_OBJECT

public:
    CopyFileThread(const QString &source, const QString &destination, QObject *parent);
    CopyFileThread(QObject *parent);

    void setSource(const QString &source);
    void setDestination(const QString &destination);

protected:
    virtual void run();

private:
    QString m_source;
    QString m_destination;
};

#endif
