#ifndef _ICONSVIEWCONTROLLER_HPP
#define _ICONSVIEWCONTROLLER_HPP

#include <QObject>

class QModelIndex;
class QFileInfo;
class QPoint;
class QItemSelectionModel;
class FileSystemModel;
class QItemSelection;

class IconsViewController: public QObject
{
    Q_OBJECT

public:
    IconsViewController(QObject *parent = 0);

    void setSelectionModel(QItemSelectionModel *model);

    void itemContextMenu(const QModelIndex &index, const QPoint &point);

public slots:
    void triggerItem(const QModelIndex &index);
    void selectItem(const QModelIndex &index);
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void openFile();
    void cutFile();
    void copyFile();
    void pasteFile();

private:
    FileSystemModel *model() const;

    void discardNextSelection();
    void acceptNextSelection();

private slots:
    void newFolder();
    void renameFile();
    void removeFile();
    void openPropertiesDialog();

signals:
    void itemTriggered(const QFileInfo &file);
    void itemSelected(const QModelIndex &index);

private:
    QItemSelectionModel *m_selectionModel;
    bool m_discardSelection;
};

#endif
