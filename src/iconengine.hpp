#ifndef _ICONENGINE_HPP
#define _ICONENGINE_HPP

#include <QString>

class FileItem;

class IconEngine
{
    enum Context
    {
        AnyContext,
        ApplicationContext,
        MimeTypeContext
    };

public:
    IconEngine(const QString &iconTheme, int iconSize);

    QString applicationIcon(const QString &iconName);
    QString mimeTypeIcon(const QString &mimeType, const QString &mediaType);

private:
    QString contextIconPath(Context context);

private:
    QString m_iconTheme;
    int m_iconSize;
};

#endif
