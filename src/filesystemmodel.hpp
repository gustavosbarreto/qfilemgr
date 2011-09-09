#ifndef _FILESYSTEMMODEL_HPP
#define _FILESYSTEMMODEL_HPP

#include <QFileSystemModel>

class MimeDetector;
class FileItem;

class FileSystemModel: public QFileSystemModel
{
    Q_OBJECT

public:
    enum Role
    {
        MimeTypeDescriptionRole = Qt::UserRole + 4, // Descricao do MimeType
        MimeTypeIconRole, // Icone do MimeType
        DefaultApplicationNameRole, // Nome do aplicativo padrao que abre o arquivo
        DefaultApplicationDesktopFileRole, // Arquivo .desktop do aplicativo padrao que abre o arquivo
        DefaultApplicationIconRole, // Icone do aplicativo padrao que abre o arquivo
        FileItemRole // MAGIC
    };

    FileSystemModel(QObject *parent = 0);
    ~FileSystemModel();

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QModelIndex setRootPathOwn(const QString &newPath);

private slots:
    void deleteUnusedFileItems();

private:
    MimeDetector *mimeDetector;
};

#endif
