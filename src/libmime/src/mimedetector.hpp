#ifndef _MIMEDETECTOR_HPP
#define _MIMEDETECTOR_HPP

#include <QMap>
#include <QStringList>

#include "weightedmimetype.hpp"

class QDataStream;
class MimeType;

class MimeDetector
{
public:
    MimeDetector(const QString mimeCacheFile);
    ~MimeDetector();

    MimeType *mimeTypeForFileName(const QString &fileName);

private:
    QStringList lookupMimeTypesForGlobFileName(const QString &fileName);
    QStringList lookupGlobLiteral(const QString &fileName);
    QStringList lookupGlobSuffix(QString fileName, bool ignoreCase, int len);
    QStringList lookupGlobFileNameMatch(const QString &fileName);
    QStringList lookupGlobNodeSuffix(const QString &fileName,
                                                 int numEntries, int offset,
                                                 bool ignoreCase, int len, QString pattern = QString());

    void initMimeTypes();
    void addMimeType(const MimeType &mimeType);

    QString getString(int offset, bool regExp = false);
    
    quint32 aliasListOffset();
    quint32 aliasesCount(int offset);
    quint32 literalListOffset();
    quint32 literalListCount(int offset);
    quint32 reverseSuffixTreeOffset();
    quint32 reverseSuffixTreeCount(int offset);
    quint32 globListOffset();
    quint32 globListCount(int offset);
    quint32 iconListOffset();
    quint32 iconListCount(int offset);

private:
    QDataStream *stream;
    QMap<QString, QStringList> m_mimeTypes;
    QMap<QString, MimeType *> m_mimeTypess;
};

#endif
