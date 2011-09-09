#include "iconengine.hpp"
#include "filemanager.hpp"

#include <QMap>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QSize>
#include <QVariant>
#include <QFileInfo>

// Usado para armazenar os icones genericos do 'mime/generic-icons'
static QMap<QString, QString> genericIconsMap;

static void updateGenericIconsMap()
{
    QFile file("/usr/share/mime/generic-icons");
    file.open(QFile::ReadOnly | QIODevice::Text);

    QTextStream stream(&file);
    while (!stream.atEnd())
    {
        QStringList parts = stream.readLine().split(':');
        if (parts.size() < 2)
            continue;
        genericIconsMap.insert(parts.at(0), parts.at(1));
    }

    file.close();
}

IconEngine::IconEngine(const QString &iconTheme, int iconSize):
    m_iconTheme(iconTheme),
    m_iconSize(iconSize)
{
}

QString IconEngine::applicationIcon(const QString &iconName)
{
    QString iconPath;

    if (QFileInfo(iconName).isAbsolute()) // Path fixo para o icone?
        return iconName;

    // Tenta icone especifico para o aplicativo
    if (iconName.contains('.'))
        iconPath = contextIconPath(AnyContext) + iconName;
    else
    {
        static const char *ext[2] = { ".png", ".xpm" }; // PNG tem prioridade sobre xpm
        for (int i = 0; i < 2; ++i)
        {
            iconPath = contextIconPath(AnyContext) + iconName + ext[i];
            if (QFile::exists(iconPath))
                break;
        }
    }

    // Se nao tenta pegar o icone do thema
    if (!QFile::exists(iconPath))
    {
        iconPath = contextIconPath(ApplicationContext) + iconName + ".png";
        if (!QFile::exists(iconPath))
            iconPath.clear();
    }

    return iconPath;
}

QString IconEngine::mimeTypeIcon(const QString &mimeType, const QString &mediaType)
{
    // Cria cache dos icones genericos se necessario
    if (genericIconsMap.isEmpty())
        updateGenericIconsMap();

    QString iconName = QString(mimeType).replace('/', '-'); // Padrao XDG
    QString iconCanonicalPath = contextIconPath(MimeTypeContext);
    QString iconPath = iconCanonicalPath + iconName + ".png"; // Icone principal usando o mimetype

    // Usa icone generico se icone principal nao for encontrado
    if (!QFile::exists(iconPath))
    {
        if (!genericIconsMap.value(iconName).isEmpty())
            iconPath = iconCanonicalPath + genericIconsMap.value(iconName) + ".png";
    }

    /* Se um icone generico não eh encontrado entao o icone deve ser
       o tipo de midia do item superior ("video" em "video/ogg") mais
       a string "-x-generic" ("video-x-generic") */
    if (!QFile::exists(iconPath))
    {
        static const char* const fallbacks[] = {
            "text", "application", "image", "audio",
            "inode", "video", "message", "model", "multipart",
            "x-content", "x-epoc"
        };

        for (uint i = 0; i < sizeof(fallbacks) / sizeof(*fallbacks); ++i)
        {
            if (mediaType == fallbacks[i])
            {
                iconPath = iconCanonicalPath + mediaType + "x-generic.png";
                break;
            }
        }
    }

    return iconPath;
}

QString IconEngine::contextIconPath(Context context)
{
    QString canonicalPath = QString("/usr/share/icons/%1/%2x%3")
        .arg(m_iconTheme)
        .arg(m_iconSize)
        .arg(m_iconSize);

    switch (context)
    {
        case AnyContext:
            canonicalPath = "/usr/share/pixmaps/";
            break;
        case ApplicationContext:
            canonicalPath.append("/apps/");
            break;
        case MimeTypeContext:
            canonicalPath.append("/mimetypes/");
            break;
    }

    return canonicalPath;
}
