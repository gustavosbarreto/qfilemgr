#include "mimeutils.hpp"

#include <QFile>
#include <QMap>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

static QMap<QString, QString> genericIcons;
static QMap<QString, QString> iconList;

namespace MIMEUtils
{
    const QString getIconForMimeType(const QString &mimeType, const QString &mediaType)
    {
        if (genericIcons.isEmpty())
        {
            QFile file("/usr/share/mime/generic-icons");
            file.open(QFile::ReadOnly | QIODevice::Text);

            QTextStream stream(&file);
            while (!stream.atEnd())
            {
                QStringList parts = stream.readLine().split(':');
                genericIcons.insert(parts.at(0), parts.at(1));
            }

            file.close();
        }

        QMap<QString, QString>::iterator it = iconList.find(mimeType);
        if (it == iconList.end())
        {
            QString prefix = "/usr/share/icons/oxygen/48x48/mimetypes/";
            QString icon = QString(mimeType).replace('/', '-') + ".png"; // Adaptacao geral aos nomes de icones

            if (!QFile::exists(prefix + icon))
            {
                // Adapta aos nomes usado nos icones do KDE
                if (mediaType == "image")
                    icon = "image-x-generic.png";
                else if (mediaType == "video")
                    icon = "video-x-generic.png";
                else if (mediaType == "audio")
                    icon = "audio-x-generic.png";
            }

            // Icones genericos
            if (!QFile::exists(prefix + icon))
            {
                if (!genericIcons.value(mimeType).isEmpty())
                    icon = genericIcons.value(mimeType) + ".png";
            }

            if (!QFile::exists(prefix + icon))
                icon = "unknown";

            return iconList.insert(mimeType, prefix + icon).value();
        }
        else
        {
            return it.value();
        }
    }
}
