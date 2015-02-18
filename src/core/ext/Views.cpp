#include "Views.h"
#include <QStyleOptionButton>

namespace Qi
{

void ViewCallback::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const
{
    Q_ASSERT(drawFunction);
    drawFunction(painter, ctx, cache, showTooltip);
}

QSize ViewCallback::sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const
{
    if (sizeFunction)
        return sizeFunction(ctx, item, sizeMode);
    return View::sizeImpl(ctx, item, sizeMode);
}

bool ViewCallback::textImpl(const ItemID& item, QString& txt) const
{
    if (textFunction)
        return textFunction(item, txt);
    return View::textImpl(item, txt);
}

} // end namespace Qi
