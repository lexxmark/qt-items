#ifndef QI_LAYOUT_H
#define QI_LAYOUT_H

#include "View.h"

namespace Qi
{

class QI_EXPORT Layout: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Layout)

public:
    Layout();
    virtual ~Layout();
    
    void doLayout(const View& view, const CellID& cell, DrawContext& dc, QRect& availableRect, QRect& viewRect) const { doLayoutImpl(view, cell, dc, availableRect, viewRect); }
    void doExpandSize(const View& view, const CellID& cell, DrawContext& dc, QSize& size) const { doExpandSizeImpl(view, cell, dc, size); }
    
Q_SIGNALS:
    void layoutChanged(const Layout*);

protected:
    virtual void doLayoutImpl(const View& view, const CellID& cell, DrawContext& dc, QRect& availableRect, QRect& viewRect) const = 0;
    virtual void doExpandSizeImpl(const View& view, const CellID& cell, DrawContext& dc, QSize& size) const = 0;
};

} // end namespace Qi

#endif // QI_LAYOUT_H
