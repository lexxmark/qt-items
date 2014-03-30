#include "Lines.h"

namespace Qi
{

Lines::Lines()
    : m_count(0)
{
}

Lines::~Lines()
{
}

quint32 Lines::count() const
{
    return m_count;
}

void Lines::setCount(quint32 count)
{
    if (m_count == count)
    {
        emit linesChanged(*this, ChangeReasonLinesCountWeak);
        return;
    }
    
    // if there are different visibilities
    if (m_linesVisibility.length() > 1)
    {
        m_linesVisibility.resize(count);
        // mark extra lines visibility as visible
        for (quint32 i = m_count; i < count; ++i)
        {
            m_linesVisibility[i] = true;
        }
    }
        
    // if there are different visibilities
    if (m_linesSizes.length() > 1)
    {
        m_linesSizes.resize(count);
        // set extra lines size to default
        for (quint32 i = m_count; i < count; ++i)
        {
            m_linesSizes[i] = 50;
        }
    }
        
    // invalidate permutation
    m_permutation.clear();

    m_count = count;
    emit linesChanged(*this, ChangeReasonLinesCount|ChangeReasonLinesCountWeak);
}

bool Lines::isLineVisible(quint32 i) const
{
    return m_linesVisibility[i];
}

void Lines::setLineVisible(quint32 i, bool isVisible)
{
    bool& visibility = m_linesVisibility[i];
    if (visibility != isVisible)
    {
        visibility = isVisible;
        emit linesChanged(*this, ChangeReasonLineVisibility);
    }
}

quint32 Lines::lineSize(quint32 i) const
{
    return m_linesSizes[i];
}

void Lines::setLineSize(quint32 i, quint32 size)
{
    quint32& lineSize = m_linesSizes[i];
    if (lineSize != size)
    {
        lineSize = size;
        emit linesChanged(*this, ChangeReasonLineSize);
    }
}

const QVector<quint32>& Lines::permutation() const
{
    return m_permutation;
}

void Lines::setPermutation(const QVector<quint32>& permutation)
{
    // weak check
    Q_ASSERT((quint32)permutation.length() == m_count);
    
    m_permutation = permutation;
    emit linesChanged(*this, ChangeReasonLineOrder);
}

quint32 Lines::toVisible(quint32 i) const
{
    if (!isLineVisible(i))
        return Invalid;
    
    return m_permutation[i];
}

quint32 Lines::toAbsolute(quint32 i) const
{
    if (m_visToAbs.isEmpty())
    {
        QVector<quint32> visToAbs;
        if (m_permutation.isEmpty())
        {
            for (quint32 i = 0; i < m_count; ++i)
            {
                if (isLineVisible(i))
                    visToAbs.append(i);
            }
        }
        else
        {
            for (quint32 i = 0; i < m_count; ++i)
            {
                if (isLineVisible(i))
                    visToAbs.append(m_permutation[i]);
            }
        }
    }
    return m_visToAbs[i];
}

quint32 Lines::sizeAtLine(quint32 i) const
{
    Q_ASSERT(false);
    return 0;
}

}
