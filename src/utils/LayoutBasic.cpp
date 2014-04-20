#include "LayoutBasic.h"

namespace Qi
{

LayoutAll::LayoutAll()
{
}

void LayoutAll::doLayoutImpl(const View* view, const CellID& cell, const QWidget* widget, QRect& availableRect, QRect& viewRect) const
{
    viewRect = availableRect;
    availableRect.setLeft(availableRect.right());
}

void LayoutAll::doExpandSizeImpl(const View* view, const CellID& cell, const QWidget* widget, QSize& size) const
{
    size = view->sizeHint(widget, cell);
}

LayoutLeft::LayoutLeft()
{
}

void LayoutLeft::doLayoutImpl(const View* view, const CellID& cell, const QWidget* widget, QRect& availableRect, QRect& viewRect) const
{
    QSize viewSizeHint = view->sizeHint(widget, cell);
    
    viewRect = availableRect;
    viewRect.setRight(qMin(viewRect.right(), viewRect.left() + viewSizeHint.width()));
    
    availableRect.setLeft(viewRect.right());
}

void LayoutLeft::doExpandSizeImpl(const View* view, const CellID& cell, const QWidget* widget, QSize& size) const
{
    QSize viewSize = view->sizeHint(widget, cell);
    size.rwidth() += viewSize.width();
    size.rheight() = qMax(size.height(), viewSize.height());
}

} // end namespace Qi
