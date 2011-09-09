#ifndef _HISTORYLIST_HPP
#define _HISTORYLIST_HPP

#include <QMap>
#include <QString>

class HistoryList: public QMap<int, QString>
{
public:
    HistoryList();

    void append(const QString &uri);

    QString goBack();
    QString goForward();

    bool canBack() const;
    bool canForward() const;

private:
    int m_currentIndex;
};

#endif
