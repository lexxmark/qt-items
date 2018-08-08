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

#include "View.h"
#include "Layout.h"
#include "core/ext/ControllerMouseMultiple.h"
#include "core/ext/ControllerMousePushable.h"

namespace Qi
{

View2::View2()
{
}

View2::~View2()
{
}

void View2::setController(SharedPtr<ControllerMouse> controller)
{
    if (m_controller == controller)
        return;

    m_controller = controller;
    emitViewChanged(ChangeReasonViewController);
}

void View2::addController(SharedPtr<ControllerMouse> controller)
{
    if (!m_controller)
    {
        setController(controller);
    }
    else
    {
        auto controllerMultiple = qSharedPointerObjectCast<ControllerMouseMultiple>(m_controller);
        if (!controllerMultiple)
        {
            controllerMultiple.reset(new ControllerMouseMultiple());
            controllerMultiple->addController(m_controller);
        }

        controllerMultiple->addController(controller);
        setController(controllerMultiple);
    }
}

void View2::draw(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const
{
    Q_ASSERT(cache.cacheView.view() == this);

    drawImpl(painter, ctx, cache, showTooltip);

    if (showTooltip && tooltipTextCallback)
        *showTooltip = true;
}

bool View2::tooltipText(ID id, QString& text) const
{
    if (tooltipTextCallback)
        return tooltipTextCallback(id, text);

    return tooltipTextImpl(id, text);
}

void View2::emitViewChanged(ChangeReason reason)
{
    emit viewChanged(this, reason);
}

void View2::addViewImpl(ID /*id*/, QVector<const View2*>& views) const
{
    views.append(this);
}

CacheView2* View2::addCacheViewImpl(const Layout& layout, const GuiContext& ctx, ID id, QVector<CacheView2>& cacheViews, QRect& itemRect, QRect* visibleItemRect) const
{
    QRect viewRect(0, 0, 0, 0);
    if (!layout.doLayout(*this, ctx, id, ViewSizeModeExact, viewRect, itemRect, visibleItemRect))
        return nullptr;

    CacheView2 cacheView(this, viewRect);
    cacheViews.append(cacheView);
    return &cacheViews.back();
}

SharedPtr<View> View2::createCacheViewImpl(const View* parent, ID id, const GuiContext& ctx) const
{
    return makeShared<ViewProxy>(parent, id, this, ctx);
}

QSize View2::sizeImpl(const GuiContext& /*ctx*/, ID /*id*/, ViewSizeMode /*sizeMode*/) const
{
    // it should not be requested
    return QSize(0, 0);
}

void View2::setTooltipText(const QString& text)
{
    tooltipTextCallback = [text] (ID /*id*/, QString& itemText) {
        itemText = text;
        return true;
    };
}

ViewProxy::ViewProxy(const View* parent, ID id, const View2* view2, const GuiContext& ctx)
    : View(parent, id),
      m_view2(view2),
      m_ctx(ctx)
{
    Q_ASSERT(m_view2);
}

QSize ViewProxy::contentSizeImpl(ViewSizeMode sizeMode) const
{
    return m_view2->size(m_ctx, id(), sizeMode);
}

void ViewProxy::drawImpl(QPainter* /*painter*/) const
{
    //m_view->draw(painter, m_ctx, *this);
}

bool ViewProxy::contentAsTextImpl(QString& txt) const
{
    return m_view2->text(id(), txt);
}

bool ViewProxy::tooltipImpl(QPoint /*point*/, TooltipInfo& tooltip) const
{
    if (m_view2->tooltipText(id(), tooltip.text))
    {
        tooltip.rect = rect();
        return true;
    }

    return false;
}

} // end namespace Qi
