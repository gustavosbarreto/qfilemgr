#ifndef _GENERALFILEPROPERTIESPAGE_HPP
#define _GENERALFILEPROPERTIESPAGE_HPP

#include "filepropertiespage.hpp"

#include "ui_generalfilepropertiespage.h"

class QModelIndex;
class DirectorySizeCalculator;

class GeneralFilePropertiesPage: public FilePropertiesPage
{
    Q_OBJECT

public:
    GeneralFilePropertiesPage(const QModelIndex &index, QWidget *parent);
    ~GeneralFilePropertiesPage();

    virtual void save();

private:
    const QString fileSizeToString(quint64 bytes);

private slots:
    void directorySizeCalculated();
    void updateSize(quint64 size);

private:
    Ui::GeneralFilePropertiesPage ui;
    DirectorySizeCalculator *m_directorySizeCalculator;
};

#endif

