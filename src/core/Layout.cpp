#include "Layout.h"
#include "View.h"

namespace Qi
{

QSize Layout::ViewInfo::size() const
{
    return view.size(ctx, item, sizeMode);
}

bool Layout::doLayout(const View& view, const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode, QRect& viewRect, QRect& itemRect, QRect* visibleItemRect) const
{
    ViewInfo vi(view, ctx, item, sizeMode);
    LayoutInfo li(viewRect, itemRect);
    if (!doLayoutImpl(vi, li))
        return false;

    if (isFloat() && visibleItemRect)
    {
        LayoutInfo li(viewRect, *visibleItemRect);
        if (!doLayoutImpl(vi, li))
            return false;
    }

    return true;
}

void Layout::expandSize(const View& view, const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode, QSize& size) const
{
    if (isTransparent())
        return;

    expandSizeImpl(ViewInfo(view, ctx, item, sizeMode), size);
}

bool Layout::isFinal() const
{
    return isFinalImpl();
}

} // end namespace Qi
