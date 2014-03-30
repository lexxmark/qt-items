#ifndef QI_LAYOUT_BASIC_H
#define QI_LAYOUT_BASIC_H

#include "Layout.h"

namespace Qi
{

class QI_EXPORT LayoutAll: public Layout
{
    Q_OBJECT
    Q_DISABLE_COPY(LayoutAll)

public:
    LayoutAll();

protected:
    void doLayoutImpl(const View& view, const CellID& cell, DrawContext& dc, QRect& availableRect, QRect& viewRect) const override;
    void doExpandSizeImpl(const View& view, const CellID& cell, DrawContext& dc, QSize& size) const override;
};

class QI_EXPORT LayoutLeft: public Layout
{
    Q_OBJECT
    Q_DISABLE_COPY(LayoutLeft)

public:
    LayoutLeft();

protected:
    void doLayoutImpl(const View& view, const CellID& cell, DrawContext& dc, QRect& availableRect, QRect& viewRect) const override;
    void doExpandSizeImpl(const View& view, const CellID& cell, DrawContext& dc, QSize& size) const override;
};

} // end namespace Qi

#endif // QI_LAYOUT_BASIC_H
