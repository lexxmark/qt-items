#include "Lines.h"

namespace Qi
{

static const bool DefaultVisibility = true;
static const quint32 DefaultSize = 50;

Lines::Lines()
    : m_count(0),
      m_isConvertersValid(false),
      m_convertersCase(CC_INVALID),
      m_isSizeAtLineValid(false)
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
    
    if (count == 0)
    {
        m_linesVisibility.clear();
        m_linesSizes.clear();
        m_permutation.clear();
    }
    else
    {
        // init lines visibilities
        if (m_linesVisibility.isEmpty())
        {
            // set default visibility to all lines
            m_linesVisibility.append(DefaultVisibility);
        }
        // if there are different visibilities
        else if (m_linesVisibility.length() > 1)
        {
            m_linesVisibility.resize(count);
            // init extra lines with default visibility
            for (quint32 i = m_count; i < count; ++i)
            {
                m_linesVisibility[i] = DefaultVisibility;
            }
        }
        
        // init lines sizes
        if (m_linesSizes.isEmpty())
        {
            // set default size to all lines
            m_linesSizes.append(DefaultSize);
        }
        // if there are different sizes
        else if (m_linesSizes.length() > 1)
        {
            m_linesSizes.resize(count);
            // set extra lines size to default
            for (quint32 i = m_count; i < count; ++i)
            {
                m_linesSizes[i] = DefaultSize;
            }
        }
        
        // invalidate order
        m_permutation.clear();
    }

    invalidateConverters();

    m_count = count;
    emit linesChanged(*this, ChangeReasonLinesCount|ChangeReasonLinesCountWeak);
}

quint32 Lines::visibleCount() const
{
    validateConverters();

    if (m_convertersCase == CC_TRIVIAL)
        return m_count;
    else
        return (quint32)m_visToAbs.length();
}

quint32 Lines::visibleSize() const
{
    quint32 vCount = visibleCount();
    if (vCount == 0)
        return 0;
    else
        return sizeAtVisLine(vCount - 1);
}

bool Lines::isLineVisible(quint32 i) const
{
    if (m_linesVisibility.size() == 1)
        return m_linesVisibility.first();
    else
        return m_linesVisibility[i];
}

void Lines::setLineVisible(quint32 i, bool isVisible)
{
    // in case all visibilities are the same
    if ((m_linesVisibility.length() == 1) && (m_count != 1))
    {
        // do nothing if visibility won't change
        if (m_linesVisibility.first() == isVisible)
            return;
        
        // expand visibilities
        m_linesVisibility.fill(m_linesVisibility.first(), m_count);
    }
    
    bool& visibility = m_linesVisibility[i];
    if (visibility != isVisible)
    {
        visibility = isVisible;
        invalidateConverters();
        emit linesChanged(*this, ChangeReasonLineVisibility);
    }
}

void Lines::setAllLinesVisible(bool isVisible)
{
    QVector<bool> linesVisibility(1, isVisible);
    if (linesVisibility != m_linesVisibility)
    {
        m_linesVisibility.swap(linesVisibility);
        invalidateConverters();
        emit linesChanged(*this, ChangeReasonLineVisibility);
    }
}

bool Lines::isVisibilitySimilar() const
{
    return m_linesVisibility.length() == 1;
}

quint32 Lines::lineSize(quint32 i) const
{
    if (m_linesSizes.length() == 1)
        return m_linesSizes.first();
    else
        return m_linesSizes[i];
}

void Lines::setLineSize(quint32 i, quint32 size)
{
    // in case all sizes are the same
    if ((m_linesSizes.length() == 1) && (m_count != 1))
    {
        // do nothing if size won't change
        if (m_linesSizes.first() == size)
            return;
        
        // expand sizes
        m_linesSizes.fill(m_linesSizes.first(), m_count);
    }

    quint32& lineSize = m_linesSizes[i];
    if (lineSize != size)
    {
        lineSize = size;
        emit linesChanged(*this, ChangeReasonLineSize);
    }
}

void Lines::setAllLinesSize(quint32 size)
{
    QVector<quint32> linesSizes(1, size);
    if (linesSizes != m_linesSizes)
    {
        m_linesSizes.swap(linesSizes);
        emit linesChanged(*this, ChangeReasonLineSize);
    }
}

bool Lines::isSizeSimilar() const
{
    return m_linesSizes.length() == 1;
}

const QVector<quint32>& Lines::permutation() const
{
    return m_permutation;
}

void Lines::setPermutation(const QVector<quint32>& permutation)
{
    // at least sizes should be equal
    Q_ASSERT((quint32)permutation.length() == m_count);
    
    m_permutation = permutation;
    invalidateConverters();
    emit linesChanged(*this, ChangeReasonLineOrder);
}

quint32 Lines::toVisible(quint32 i) const
{
    validateConverters();
    
    if (m_convertersCase == CC_ALL_HIDDEN)
        return Invalid;
    else if (m_convertersCase == CC_TRIVIAL)
        return i;
    else
        return m_absToVis[i];
}

quint32 Lines::toVisibleSafe(quint32 i) const
{
    if (i >= m_count)
        return Invalid;
    return toVisible(i);
}

quint32 Lines::toAbsolute(quint32 i) const
{
    validateConverters();
    if (m_convertersCase == CC_TRIVIAL)
        return i;
    else
        return m_visToAbs[i];
}

quint32 Lines::toAbsoluteSafe(quint32 i) const
{
    if (i >= visibleCount())
        return Invalid;
    else
        return toAbsolute(i);
}

void Lines::invalidateConverters()
{
    m_isConvertersValid = false;
    m_convertersCase = CC_INVALID;
    m_isSizeAtLineValid = false;
}

void Lines::validateConverters() const
{
    if (m_isConvertersValid)
        return;
    
    m_isConvertersValid = true;
    
    if (m_count == 0)
    {
        m_visToAbs.clear();
        m_absToVis.clear();
        m_convertersCase = CC_ALL_HIDDEN;
    }
    else if (m_permutation.isEmpty() && isVisibilitySimilar())
    {
        m_visToAbs.clear();
        m_absToVis.clear();
        m_convertersCase = (isLineVisible(0)) ? CC_TRIVIAL : CC_ALL_HIDDEN;
    }
    else
    {
        // find inverted permutation
        QVector<quint32> permutationInv(m_count);
        if (m_permutation.isEmpty())
        {
            for (quint32 i = 0; i < m_count; ++i)
                permutationInv[i] = i;
        }
        else
        {
            Q_ASSERT((quint32)m_permutation.length() == m_count);
            for (quint32 i = 0; i < m_count; ++i)
                permutationInv[m_permutation[i]] = i;
        }
        
        // init visible to absolute conversion
        if (isVisibilitySimilar())
        {
            // all lines visible
            if (isLineVisible(0))
            {
                m_visToAbs.swap(permutationInv);
                m_convertersCase = CC_MAPPED;
            }
            // all lines invisible
            else
            {
                m_visToAbs.clear();
                m_convertersCase = CC_ALL_HIDDEN;
            }
        }
        else
        {
            m_visToAbs.clear();
            for (quint32 i = 0; i < m_count; ++i)
            {
                if (isLineVisible(permutationInv[i]))
                    m_visToAbs.append(permutationInv[i]);
            }
            m_convertersCase = CC_MAPPED;
        }
        
        // init absolute to visible conversion
        if (m_visToAbs.isEmpty())
        {
            m_absToVis.clear();
            m_convertersCase = CC_ALL_HIDDEN;
        }
        else
        {
            m_absToVis.fill(Invalid, m_count);
            for (quint32 i = 0, n = m_visToAbs.length(); i < n; ++i)
            {
                m_absToVis[m_visToAbs[i]] = i;
            }
        }
    }
    
    Q_ASSERT(m_convertersCase != CC_INVALID);
}

void Lines::validateSizeAtLine() const
{
    validateConverters();
    if (m_convertersCase != CC_MAPPED)
    {
        m_sizeAtLine.clear();
    }
    else
    {
        quint32 totalSize = 0;
        m_sizeAtLine.resize(m_visToAbs.size());
        for (quint32 i = 0, n = m_sizeAtLine.length(); i < n; ++i)
        {
            totalSize = totalSize + lineSize(m_visToAbs[i]);
            m_sizeAtLine[i] = totalSize;
        }
    }
}


quint32 Lines::sizeAtVisLine(quint32 i) const
{
    validateSizeAtLine();
    
    if (m_convertersCase == CC_TRIVIAL)
        return (i + 1) * lineSize(0);
    else
        return m_sizeAtLine[i];
}

static quint32 findValue(const QVector<quint32>& values, int a, int b, quint32 value)
{
    if (a == b)
        return a;
    
    if ((b - a) == 1)
    {
        quint32 aValue = values[a];
        if (value < aValue)
            return a;
        else
            return b;
    }
    
    int c = a + (b - a)/2;
    quint32 cValue = values[c];
    if (value < cValue)
        return findValue(values, a, c, value);
    else
        return findValue(values, c, b, value);
}

quint32 Lines::findVisLine(quint32 sizeAtLine) const
{
    validateSizeAtLine();
    
    if (m_convertersCase == CC_ALL_HIDDEN)
        return Invalid;
    else if (m_convertersCase == CC_TRIVIAL)
        return qMin(sizeAtLine / lineSize(0), visibleCount() - 1);
    else
    {
        if (sizeAtLine >= m_sizeAtLine.last())
            return m_sizeAtLine.length() - 1;
        else
            return findValue(m_sizeAtLine, 0, m_sizeAtLine.length() - 1, sizeAtLine);
    }
}

} // end namespace Qi
