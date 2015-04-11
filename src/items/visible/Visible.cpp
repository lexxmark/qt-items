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

#include "Visible.h"
#include <QCoreApplication>

namespace Qi
{

ViewVisible::ViewVisible(const QSharedPointer<View>& sourceView, bool reserveSize)
    : m_sourceView(sourceView),
      m_reserveSize(reserveSize)
{
    Q_ASSERT(m_sourceView);
    connect(m_sourceView.data(), &View::viewChanged, this, &ViewVisible::onSourceViewChanged);
}

void ViewVisible::notifyVisibilityChanged()
{
    emit viewChanged(this, ChangeReasonViewSize);
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

void ViewVisible::onSourceViewChanged(const View* view, ChangeReason reason)
{
    Q_UNUSED(view);
    Q_ASSERT(view == m_sourceView.data());

    emit viewChanged(this, reason);
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
    Q_UNUSED(e);
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
