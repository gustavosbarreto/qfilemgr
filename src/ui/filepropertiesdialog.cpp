#include "filepropertiesdialog.hpp"
#include "generalfilepropertiespage.hpp"
//#include "sharefilepropertiespage.hpp"

#include <QSettings>

FilePropertiesDialog::FilePropertiesDialog(const QModelIndex &index, QWidget *parent):
    QDialog(parent)
{
    ui.setupUi(this);
    ui.tabWidget->addTab(new GeneralFilePropertiesPage(index, this), trUtf8("General"));

    QSettings s(QSettings::SystemScope, "ossystems", "sharing", this);
//    if (s.value("General/Usershares").toBool())
//        ui.tabWidget->addTab(new ShareFilePropertiesPage(index, this), trUtf8("Sharing"));
}

int FilePropertiesDialog::exec()
{
    int result = QDialog::exec();
    if (!result)
        return result;
    return result;
}
