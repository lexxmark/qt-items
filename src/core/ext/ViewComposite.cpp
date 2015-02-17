#include "ViewComposite.h"

namespace Qi
{

ViewComposite::ViewComposite(const QVector<ViewSchema>& subViews, const QMargins& margins)
    : m_subViews(subViews),
      m_margins(margins)
{
    Q_ASSERT(!m_subViews.isEmpty());
    connectSubViews();
}

ViewComposite::ViewComposite(const QSharedPointer<View>& subView, const QMargins& margins)
    : m_margins(margins)
{
    m_subViews.push_back(ViewSchema(makeLayoutClient(), subView));
    connectSubViews();
}

ViewComposite::~ViewComposite()
{
    disconnectSubViews();
}

void ViewComposite::setMargins(const QMargins& margins)
{
    if (m_margins != margins)
    {
        m_margins = margins;
        emit viewChanged(this, ChangeReasonViewSize);
    }
}

void ViewComposite::addViewImpl(const ItemID& item, QVector<const View*>& views) const
{
    View::addViewImpl(item, views);
    for (const auto& subView: m_subViews)
    {
        subView.view->addView(item, views);
    }
}

CacheView* ViewComposite::addCacheViewImpl(const Layout& layout, const GuiContext& ctx, const ItemID& item, QVector<CacheView>& cacheViews, QRect& itemRect, QRect* visibleItemRect) const
{
    CacheView* selfCacheView = View::addCacheViewImpl(layout, ctx, item, cacheViews, itemRect, visibleItemRect);
    if (!selfCacheView)
        return selfCacheView;

    QRect localRect = selfCacheView->rect().marginsRemoved(m_margins);

    for (const auto& subView: m_subViews)
    {
        subView.view->addCacheView(*subView.layout, ctx, item, selfCacheView->rSubViews(), localRect, visibleItemRect);
    }

    return selfCacheView;
}

QSize ViewComposite::sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const
{
    QSize size(0, 0);

    // iterate in reversed order to handle client view first
    for (int i = m_subViews.size() - 1; i >= 0; --i)
    {
        const auto& subView = m_subViews[i];
        subView.layout->expandSize(*subView.view, ctx, item, sizeMode, size);
    }

    return QSize(size.width() + m_margins.left() + m_margins.right(),
                 size.height() + m_margins.top() + m_margins.bottom());
}

void ViewComposite::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* /*showTooltip*/) const
{
    for (const auto& subCacheView: cache.cacheView.subViews())
    {
        subCacheView.draw(painter, ctx, cache.item, cache.itemRect, cache.visibleRect);
    }
}

void ViewComposite::cleanupDrawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache) const
{
    // restore draw state in reversed order
    const auto& subCacheViews = cache.cacheView.subViews();
    for (int i = subCacheViews.size() - 1; i >= 0; --i)
    {
        const auto& subCacheView = subCacheViews[i];
        subCacheView.cleanupDraw(painter, ctx, cache.item, cache.itemRect, cache.visibleRect);
    }
}

bool ViewComposite::textImpl(const ItemID& item, QString& txt) const
{
    bool isAnySubText = false;

    for (const auto& subView: m_subViews)
    {
        QString subText;
        if (subView.view->text(item, subText))
        {
            isAnySubText = true;
            txt += subText;
        }
    }

    return isAnySubText;
}

void ViewComposite::connectSubViews()
{
    for (const auto& view: m_subViews)
    {
        connect(view.view.data(), &View::viewChanged, this, &ViewComposite::onSubViewChanged);
    }
}

void ViewComposite::disconnectSubViews()
{
    for (const auto& view: m_subViews)
    {
        disconnect(view.view.data(), &View::viewChanged, this, &ViewComposite::onSubViewChanged);
    }
}

void ViewComposite::onSubViewChanged(const View* /*view*/, ChangeReason reason)
{
    emit viewChanged(this, reason);
}

} // end namespace Qi
