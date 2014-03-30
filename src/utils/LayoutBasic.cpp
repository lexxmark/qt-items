#include "LayoutBasic.h"

namespace Qi
{

LayoutAll::LayoutAll()
{
}

void LayoutAll::doLayoutImpl(const View& view, const CellID& cell, DrawContext& dc, QRect& availableRect, QRect& viewRect) const
{
    viewRect = availableRect;
    availableRect.setLeft(availableRect.right());
}

void LayoutAll::doExpandSizeImpl(const View& view, const CellID& cell, DrawContext& dc, QSize& size) const
{
    size = view.sizeHint(dc, cell);
}

LayoutLeft::LayoutLeft()
{
}

void LayoutLeft::doLayoutImpl(const View& view, const CellID& cell, DrawContext& dc, QRect& availableRect, QRect& viewRect) const
{
    QSize viewSizeHint = view.sizeHint(dc, cell);
    
    viewRect = availableRect;
    viewRect.setRight(qMin(viewRect.right(), viewRect.left() + viewSizeHint.width()));
    
    availableRect.setLeft(viewRect.right());
}

void LayoutLeft::doExpandSizeImpl(const View& view, const CellID& cell, DrawContext& dc, QSize& size) const
{
    QSize viewSize = view.sizeHint(dc, cell);
    size.rwidth() += viewSize.width();
    size.rheight() = qMax(size.height(), viewSize.height());
}

}
