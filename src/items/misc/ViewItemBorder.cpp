#include "ViewItemBorder.h"

namespace Qi
{

void validateGridColor(QColor& gridColor, const GuiContext& ctx)
{
    if (gridColor.isValid())
        return;

    auto style = ctx.style();

    QStyleOptionViewItem option;
    ctx.initStyleOption(option);
    option.widget = ctx.widget;

    const int gridHint = style->styleHint(QStyle::SH_Table_GridLineColor, &option, ctx.widget);
    gridColor = static_cast<QRgb>(gridHint);
}

ViewRowBorder::ViewRowBorder()
{
}

QSize ViewRowBorder::sizeImpl(const GuiContext& /*ctx*/, const ItemID& /*item*/, ViewSizeMode /*sizeMode*/) const
{
    return QSize(0, 1);
}

void ViewRowBorder::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* /*showTooltip*/) const
{
    validateGridColor(m_gridColor, ctx);

    QPen oldPen = painter->pen();
    painter->setPen(m_gridColor);

    const QRect& rect = cache.cacheView.rect();
    painter->drawLine(rect.right(), rect.bottom(), rect.left(), rect.bottom());

    painter->setPen(oldPen);
}

ViewColumnBorder::ViewColumnBorder()
{
}

QSize ViewColumnBorder::sizeImpl(const GuiContext& /*ctx*/, const ItemID& /*item*/, ViewSizeMode /*sizeMode*/) const
{
    return QSize(1, 0);
}

void ViewColumnBorder::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* /*showTooltip*/) const
{
    validateGridColor(m_gridColor, ctx);

    QPen oldPen = painter->pen();
    painter->setPen(m_gridColor);

    const QRect& rect = cache.cacheView.rect();
    painter->drawLine(rect.right(), rect.top(), rect.right(), rect.bottom());

    painter->setPen(oldPen);
}

} // end namespace Qi
