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
    
    bool isLineVisible(quint32 i) const;
    void setLineVisible(quint32 i, bool isVisible);
    
    quint32 lineSize(quint32 i) const;
    void setLineSize(quint32 i, quint32 size);
    
    const QVector<quint32>& permutation() const;
    void setPermutation(const QVector<quint32>& permutation);
    
    quint32 toVisible(quint32 i) const;
    quint32 toAbsolute(quint32 i) const;
    
    quint32 sizeAtLine(quint32 i) const;

Q_SIGNALS:
    void linesChanged(const Lines&, ChangeReason);

private:
    quint32 m_count;
    QVector<bool> m_linesVisibility;
    QVector<quint32> m_linesSizes;
    QVector<quint32> m_permutation;

    QVector<quint32> m_visToAbs;
    QVector<quint32> m_atLineSizes;
};

} // end namespace Qi

#endif // QI_LINES_H
