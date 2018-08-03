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

ViewVisible::ViewVisible(SharedPtr<View> sourceView, bool reserveSize)
    : m_sourceView(std::move(sourceView)),
      m_reserveSize(reserveSize)
{
    Q_ASSERT(m_sourceView);
    connect(m_sourceView.data(), &View::viewChanged, this, &ViewVisible::onSourceViewChanged);
}

void ViewVisible::notifyVisibilityChanged()
{
    emit viewChanged(this, ChangeReasonViewSize);
}

void ViewVisible::addViewImpl(ID id, QVector<const View*>& views) const
{
     if (safeIsItemVisible(id))
        m_sourceView->addView(id, views);
    else if (m_reserveSize)
        View::addViewImpl(id, views);
}

CacheView2* ViewVisible::addCacheViewImpl(const Layout& layout, const GuiContext& ctx, ID id, QVector<CacheView2>& cacheViews, QRect& itemRect, QRect* visibleItemRect) const
{
    if (safeIsItemVisible(id))
        return m_sourceView->addCacheView(layout, ctx, id, cacheViews, itemRect, visibleItemRect);
    else if (m_reserveSize)
        return View::addCacheViewImpl(layout, ctx, id, cacheViews, itemRect, visibleItemRect);
    else
        return nullptr;
}

QSize ViewVisible::sizeImpl(const GuiContext& ctx, ID id, ViewSizeMode sizeMode) const
{
    if (m_reserveSize || safeIsItemVisible(id))
        return m_sourceView->size(ctx, id, sizeMode);
    else
    {
        // framework shouldn't ask for size in this case
        Q_ASSERT(false);
        return QSize(0, 0);
    }
}

bool ViewVisible::safeIsItemVisible(ID id) const
{
    return isItemVisible ? isItemVisible(id) : false;
}

void ViewVisible::onSourceViewChanged(const View* view, ChangeReason reason)
{
    Q_UNUSED(view);
    Q_ASSERT(view == m_sourceView.data());

    emit viewChanged(this, reason);
}

ControllerMouseVisible::ControllerMouseVisible(SharedPtr<ViewVisible> view)
    : m_view(std::move(view))
{
    Q_ASSERT(m_view);

    if (m_view->isItemVisible)
    {
        auto callback = m_view->isItemVisible;
        m_view->isItemVisible = [callback, this](ID id)->bool {
            if (callback(id)) return true;
            if (auto aId = activeId())
                return (*aId == id);
            else
                return false;
        };
    }
    else
    {
        m_view->isItemVisible = [this](ID id)->bool {
            if (auto aId = activeId())
                return (*aId == id);
            else
                return false;
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
