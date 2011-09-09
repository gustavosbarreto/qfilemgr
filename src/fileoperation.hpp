#ifndef _FILEOPERATION_HPP
#define _FILEOPERATION_HPP

#include <QThread>
#include <QMetaType>

class FileOperation: public QThread
{
    Q_OBJECT

public:
    enum ExitStatus
    {
        NoStatus,
        Stopped,
        Success,
        Error
    };

    FileOperation(QObject *parent = 0);

    ExitStatus exitStatus() const;

public slots:
    virtual void stop();

protected:
    bool isToStop() const;
    void setExitStatus(ExitStatus status);

private:
    ExitStatus m_exitStatus;
    bool m_stop;

signals:
    void progress(int);
    void exited(FileOperation::ExitStatus);
};

Q_DECLARE_METATYPE(FileOperation::ExitStatus)

#endif
