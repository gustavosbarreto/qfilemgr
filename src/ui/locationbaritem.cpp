#include "locationbaritem.hpp"
#include "filemanager.hpp"
#include "filesystemmodel.hpp"

#include <QDebug>

LocationBarItem::LocationBarItem():
    button(NULL),
    arrow(NULL)
{
}

void LocationBarItem::goToUri()
{
    FileManager::self()->fileSystemModel()->setRootPathOwn(uri);
}

void LocationBarItem::goToParentFolder()
{
    FileManager::self()->fileSystemModel()->setRootPathOwn(uri.section('/', 0, uri.count('/') - 1));
}
