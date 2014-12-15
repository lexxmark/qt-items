#include "Image.h"

namespace Qi
{

ViewImage::ViewImage(const QSharedPointer<ModelImage>& model)
    : ViewModeled<ModelImage>(model)
{
}

QSize ViewImage::sizeImpl(const GuiContext& /*ctx*/, const ItemID& item, ViewSizeMode /*sizeMode*/) const
{
    QImage image = theModel()->value(item);
    return image.size();
}

void ViewImage::drawImpl(QPainter* painter, const GuiContext& /*ctx*/, const CacheContext& cache, bool* /*showTooltip*/) const
{
    QImage image = theModel()->value(cache.item);
    QRect viewRect = cache.cacheView.rect();
    int x = viewRect.left() + (viewRect.width() - image.width()) / 2;
    int y = viewRect.top() + (viewRect.height() - image.height()) / 2;

    painter->drawImage(x, y, image);
}

} // end namespace Qi
