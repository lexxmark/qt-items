#ifndef QI_LINES_H
#define QI_LINES_H

#include "../QiAPI.h"
#include <QObject>
#include <QVector>

namespace Qi
{

class QI_EXPORT Lines: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Lines)

public:
    Lines();
    virtual ~Lines();
    
    quint32 count() const;
    void setCount(quint32 count);
    
    quint32 visibleCount() const;
    quint32 visibleSize() const;
    
    bool isLineVisible(quint32 i) const;
    void setLineVisible(quint32 i, bool isVisible);
    void setAllLinesVisible(bool isVisible);
    bool isVisibilitySimilar() const;
    
    quint32 lineSize(quint32 i) const;
    void setLineSize(quint32 i, quint32 size);
    void setAllLinesSize(quint32 size);
    bool isSizeSimilar() const;
    
    // permutation[absolute] = visible
    const QVector<quint32>& permutation() const;
    void setPermutation(const QVector<quint32>& permutation);
    
    quint32 toVisible(quint32 i) const;
    quint32 toVisibleSafe(quint32 i) const;

    quint32 toAbsolute(quint32 i) const;
    quint32 toAbsoluteSafe(quint32 i) const;
    
    quint32 sizeAtVisLine(quint32 i) const;
    quint32 findVisLine(quint32 sizeAtLine) const;

Q_SIGNALS:
    void linesChanged(const Lines&, ChangeReason);

private:
    void invalidateConverters();
    void validateConverters() const;
    void validateSizeAtLine() const;
    
    quint32 m_count;
    // if (m_linesVisibility.length() == 1) => all lines has similar visibility
    QVector<bool> m_linesVisibility;
    // if (m_linesSizes.length() == 1) => all lines has similar size
    QVector<quint32> m_linesSizes;
    // if (m_permutation.isEmpty()) => lines are not ordered
    QVector<quint32> m_permutation;

    enum CONVERTER_CASE
    {
        CC_INVALID, // not initialized
        CC_ALL_HIDDEN, // lines are empty or all lines are invisible
        CC_TRIVIAL, // all lines visible and not reordered
        CC_MAPPED // m_visToAbs and m_absToVis have mappings absolute vs visible indexes
    };
    // tracks validation of m_visToAbs and m_absToVis members
    mutable bool m_isConvertersValid;
    // cases of conversion
    mutable CONVERTER_CASE m_convertersCase;
    // m_visToAbs[visibleIndex] = absoluteIndex
    mutable QVector<quint32> m_visToAbs;
    // m_absToVis[absoluteIndex] = visibleIndex | Invalid
    mutable QVector<quint32> m_absToVis;
    
    // tracks validation of the m_sizeAtLine member
    mutable bool m_isSizeAtLineValid;
    // m_sizeAtLine[visibleIndex] = distance from 0 to the end of visibleIndex line
    mutable QVector<quint32> m_sizeAtLine;
};

} // end namespace Qi 

#endif // QI_LINES_H
