/*
   Copyright (c) 2008-1015 Alex Zhondin <qtinuum.team@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "ViewItemBorder.h"

namespace Qi
{

static void validateGridColor(QColor& gridColor, const GuiContext& ctx)
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

ViewRectBorder::ViewRectBorder()
{
}

QSize ViewRectBorder::sizeImpl(const GuiContext& /*ctx*/, const ItemID& /*item*/, ViewSizeMode /*sizeMode*/) const
{
    return QSize(0, 0);
}

void ViewRectBorder::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* /*showTooltip*/) const
{
    validateGridColor(m_gridColor, ctx);

    QPen oldPen = painter->pen();
    painter->setPen(m_gridColor);

    painter->drawRect(cache.cacheView.rect());

    painter->setPen(oldPen);
}

} // end namespace Qi
