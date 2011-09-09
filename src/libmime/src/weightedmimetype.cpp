#include "weightedmimetype.hpp"

WeightedMimeType::WeightedMimeType(const QString &mimeType, const QString &pattern, int weight):
    MimeType(mimeType),
    m_pattern(pattern),
    m_weight(weight)
{
}

WeightedMimeType::WeightedMimeType(const WeightedMimeType &other):
    MimeType(other),
    m_pattern(other.m_pattern),
    m_weight(other.m_weight)
{
}

const QString WeightedMimeType::pattern() const
{
    return m_pattern;
}

int WeightedMimeType::weight() const
{
    return m_weight;
}
