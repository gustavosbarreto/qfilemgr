#ifndef _WEIGHTEDMIMETYPE_HPP
#define _WEIGHTEDMIMETYPE_HPP

#include "mimetype.hpp"

class WeightedMimeType: public MimeType
{
public:
    WeightedMimeType(const QString &mimeType, const QString &pattern, int weight);
    WeightedMimeType(const WeightedMimeType &other);

    const QString pattern() const;
    int weight() const;

private:
    QString m_pattern;
    int m_weight;
};

inline QDebug operator<<(QDebug dbg, const WeightedMimeType &mime)
{
    dbg.nospace() << "WeightedMimeType(mimeType=" << mime.mimeType() << ", mediaType=" << mime.mediaType() << ", subType=" << mime.subType() << ", pattern=" << mime.pattern() << ", weight=" << mime.weight() << ")";
    return dbg.space();
}

#endif
