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

ViewVisible::ViewVisible(const QSharedPointer<View>& sourceView)
    : m_sourceView(sourceView)
{
    connect(m_sourceView.data(), &View::viewChanged, this, &ViewVisible::onSourceViewChanged);
}

ViewVisible::ViewVisible(const QSharedPointer<View>& sourceView, const std::function<bool(const ItemID&)>& isVisibleFunction)
    : isVisibleFunction(isVisibleFunction),
      m_sourceView(sourceView)

{
    connect(m_sourceView.data(), &View::viewChanged, this, &ViewVisible::onSourceViewChanged);
}

ViewVisible::~ViewVisible()
{
    disconnect(m_sourceView.data(), &View::viewChanged, this, &ViewVisible::onSourceViewChanged);
}

void ViewVisible::onSourceViewChanged(const View* view, ChangeReason reason)
{
    Q_ASSERT(view == m_sourceView.data());
    emit viewChanged(this, reason);
}

void ViewVisible::addViewImpl(const ItemID& item, QVector<const View*>& views) const
{
    if (isVisible(item))
        m_sourceView->addView(item, views);
}

CacheView* ViewVisible::addCacheViewImpl(const Layout& layout, const GuiContext& ctx, const ItemID& item, QVector<CacheView>& cacheViews, QRect& itemRect, QRect* visibleItemRect) const
{
    if (isVisible(item))
        return m_sourceView->addCacheView(layout, ctx, item, cacheViews, itemRect, visibleItemRect);
    else
        return nullptr;
}

ViewVisibleWithSize::ViewVisibleWithSize(const QSharedPointer<View>& sourceView)
    : ViewVisible(sourceView)
{}

ViewVisibleWithSize::ViewVisibleWithSize(const QSharedPointer<View>& sourceView, const std::function<bool(const ItemID&)>& isVisibleFunction)
    : ViewVisible(sourceView, isVisibleFunction)
{}

QSize ViewVisibleWithSize::sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const
{
    // don't check isVisible
    return sourceView()->size(ctx, item, sizeMode);
}









/*

ViewText::ViewText()
{
}

void ViewText::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const
{
    QString txt;
    text(cache.item, txt);
    painter->drawText(cache.cacheView.rect(), Qt::AlignLeft | Qt::TextSingleLine | Qt::AlignVCenter, txt);
}

QSize ViewText::sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const
{
    QString txt;
    text(item, txt);
    return ctx.widget->fontMetrics().size(Qt::TextSingleLine, txt);
}

bool ViewText::textImpl(const ItemID& item, QString& text) const
{
    text = tr("Item [%1, %2]").arg(item.row).arg(item.column);
    return true;
}
*/

} // end namespace Qi
