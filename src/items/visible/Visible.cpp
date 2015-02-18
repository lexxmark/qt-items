#include "Visible.h"
#include <QCoreApplication>

namespace Qi
{

ViewVisible::ViewVisible(const QSharedPointer<View>& sourceView, bool reserveSize)
    : m_sourceView(sourceView),
      m_reserveSize(reserveSize)
{
    Q_ASSERT(m_sourceView);
}

void ViewVisible::addViewImpl(const ItemID& item, QVector<const View*>& views) const
{
    if (safeIsItemVisible(item))
        m_sourceView->addView(item, views);
    else if (m_reserveSize)
        View::addViewImpl(item, views);
}

CacheView* ViewVisible::addCacheViewImpl(const Layout& layout, const GuiContext& ctx, const ItemID& item, QVector<CacheView>& cacheViews, QRect& itemRect, QRect* visibleItemRect) const
{
    if (safeIsItemVisible(item))
        return m_sourceView->addCacheView(layout, ctx, item, cacheViews, itemRect, visibleItemRect);
    else if (m_reserveSize)
        return View::addCacheViewImpl(layout, ctx, item, cacheViews, itemRect, visibleItemRect);
    else
        return nullptr;
}

QSize ViewVisible::sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const
{
    if (m_reserveSize || safeIsItemVisible(item))
        return m_sourceView->size(ctx, item, sizeMode);
    else
    {
        // framework shouldn't ask for size in this case
        Q_ASSERT(false);
        return QSize(0, 0);
    }
}

bool ViewVisible::safeIsItemVisible(const ItemID& item) const
{
    return isItemVisible ? isItemVisible(item) : false;
}

ControllerMouseVisible::ControllerMouseVisible(const QSharedPointer<ViewVisible>& view)
    : m_view(view)
{
    Q_ASSERT(m_view);

    if (m_view->isItemVisible)
    {
        auto callback = m_view->isItemVisible;
        m_view->isItemVisible = [callback, this](const ItemID& item)->bool {
            if (callback(item)) return true;
            return (activeItem() == item);
        };
    }
    else
    {
        m_view->isItemVisible = [this](const ItemID& item)->bool {
            return (activeItem() == item);
        };
    }
}

bool ControllerMouseVisible::event(QEvent* e)
{
    Q_ASSERT(e->type() == QEvent::LayoutRequest);
    // notify view visibility has changed
    m_view->emitViewChanged(ChangeReasonViewSize);
    return true;
}

void ControllerMouseVisible::activateImpl(const ActivationInfo& activationInfo)
{
    ControllerMouse::activateImpl(activationInfo);
    notifyViewVisiblityChangedLater();
}

void ControllerMouseVisible::deactivateImpl()
{
    notifyViewVisiblityChangedLater();
    ControllerMouse::deactivateImpl();
}

void ControllerMouseVisible::notifyViewVisiblityChangedLater()
{
    // change view visibility later
    QCoreApplication::postEvent(this, new QEvent(QEvent::LayoutRequest));
}


} // end namespace Qi
