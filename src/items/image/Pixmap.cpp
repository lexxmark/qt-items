#include "Pixmap.h"

namespace Qi
{

ViewPixmap::ViewPixmap(const QSharedPointer<ModelPixmap>& model)
    : ViewModeled<ModelPixmap>(model)
{
}

QSize ViewPixmap::sizeImpl(const GuiContext& /*ctx*/, const ItemID& item, ViewSizeMode /*sizeMode*/) const
{
    QPixmap pixmap = theModel()->value(item);
    return pixmap.size();
}

void ViewPixmap::drawImpl(QPainter* painter, const GuiContext& /*ctx*/, const CacheContext& cache, bool* /*showTooltip*/) const
{
    QPixmap pixmap = theModel()->value(cache.item);
    QRect viewRect = cache.cacheView.rect();
    int x = viewRect.left() + (viewRect.width() - pixmap.width()) / 2;
    int y = viewRect.top() + (viewRect.height() - pixmap.height()) / 2;

    painter->save();
    painter->setClipRect(viewRect, Qt::IntersectClip);

    painter->drawPixmap(x, y, pixmap);

    painter->restore();
}

} // end namespace Qi
