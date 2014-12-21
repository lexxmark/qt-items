#include "ViewAlternateBackground.h"

namespace Qi
{

ViewAlternateBackground::ViewAlternateBackground()
{
}

void ViewAlternateBackground::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* /*showTooltip*/) const
{
    if (cache.item.row % 2 == 0)
    {
        painter->fillRect(cache.cacheView.rect(), ctx.palette().alternateBase());
    }
}

} // end namespace Qi
