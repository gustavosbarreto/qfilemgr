#include "detailsframe.hpp"
#include "filemanager.hpp"
#include "mimedetector.hpp"
#include "iconsviewcontroller.hpp"
#include "filesystemmodel.hpp"
#include "placesmodel.hpp"

#include <QFileSystemModel>
#include <QModelIndex>
#include <QUrl>
#include <QDebug>

DetailsFrame::DetailsFrame(QWidget *widget):
    QFrame(widget)
{
    ui.setupUi(this);

    setFrameShape(QFrame::NoFrame);

    connect(FileManager::self()->iconsViewController(),
            SIGNAL(itemSelected(const QModelIndex &)), SLOT(updateInfo(const QModelIndex &)));
}

void DetailsFrame::updateInfo(const QModelIndex &index)
{
    QString path = index.data(QFileSystemModel::FilePathRole).toString();
    QString name = QFileInfo(path).fileName();
    QModelIndex placeIndex = FileManager::self()->placesModel()->index(path);
    if (placeIndex.isValid())
        name = placeIndex.data(Qt::DisplayRole).toString();

    QFontMetrics metrics(font());
    if (name.length() > 80)
        name = metrics.elidedText(name, Qt::ElideRight, metrics.averageCharWidth() * 80);

    QPixmap icon = QPixmap(index.data(FileSystemModel::MimeTypeIconRole).toString());
    if (placeIndex.isValid())
        icon = qvariant_cast<QIcon>(placeIndex.data(Qt::DecorationRole)).pixmap(48, 48);

    ui.icon->setPixmap(icon);
    ui.name->setText(name);
    ui.type->setText(index.data(FileSystemModel::MimeTypeDescriptionRole).toString());
}
