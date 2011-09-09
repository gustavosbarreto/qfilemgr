#include "historylist.hpp"

HistoryList::HistoryList():
    m_currentIndex(0)
{
}

void HistoryList::append(const QString &uri)
{
    if (values().contains(uri))
    {
        m_currentIndex = key(uri);
        return;
    }

    insert(++m_currentIndex, uri);
}

QString HistoryList::goBack()
{
    return value(--m_currentIndex);
}

QString HistoryList::goForward()
{
    return value(++m_currentIndex);
}

bool HistoryList::canBack() const
{
    return m_currentIndex > 1;
}

bool HistoryList::canForward() const
{
    return m_currentIndex < size();
}
