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

View::View()
{
}

View::~View()
{
}

void View::setController(SharedPtr<ControllerMouse> controller)
{
    if (m_controller == controller)
        return;

    m_controller = controller;
    emitViewChanged(ChangeReasonViewController);
}

void View::addController(SharedPtr<ControllerMouse> controller)
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

void View::draw(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const
{
    Q_ASSERT(cache.cacheView.view() == this);

    drawImpl(painter, ctx, cache, showTooltip);

    if (showTooltip && tooltipTextCallback)
        *showTooltip = true;
}

bool View::tooltipText(ID id, QString& text) const
{
    if (tooltipTextCallback)
        return tooltipTextCallback(id, text);

    return tooltipTextImpl(id, text);
}

void View::emitViewChanged(ChangeReason reason)
{
    emit viewChanged(this, reason);
}

void View::addViewImpl(ID /*id*/, QVector<const View*>& views) const
{
    views.append(this);
}

CacheView2* View::addCacheViewImpl(const Layout& layout, const GuiContext& ctx, ID id, QVector<CacheView2>& cacheViews, QRect& itemRect, QRect* visibleItemRect) const
{
    QRect viewRect(0, 0, 0, 0);
    if (!layout.doLayout(*this, ctx, id, ViewSizeModeExact, viewRect, itemRect, visibleItemRect))
        return nullptr;

    CacheView2 cacheView(this, viewRect);
    cacheViews.append(cacheView);
    return &cacheViews.back();
}

SharedPtr<CacheView> View::createCacheViewImpl(const CacheView* parent, QRect rect, ID id, const GuiContext& ctx) const
{
    return makeShared<CacheViewProxy>(parent, rect, this, id, ctx);
}

QSize View::sizeImpl(const GuiContext& /*ctx*/, ID /*id*/, ViewSizeMode /*sizeMode*/) const
{
    // it should not be requested
    return QSize(0, 0);
}

void View::setTooltipText(const QString& text)
{
    tooltipTextCallback = [text] (ID /*id*/, QString& itemText) {
        itemText = text;
        return true;
    };
}

CacheViewProxy::CacheViewProxy(const CacheView* parent, QRect rect, const View* view, ID id, const GuiContext& ctx)
    : CacheView(parent, rect),
      m_view(view),
      m_id(id),
      m_ctx(ctx)
{
}

QSize CacheViewProxy::contentSizeImpl(ViewSizeMode sizeMode) const
{
    return m_view->size(m_ctx, m_id, sizeMode);
}

void CacheViewProxy::drawImpl(QPainter* painter) const
{
    //m_view->draw(painter, m_ctx, *this);
}

bool CacheViewProxy::contentAsTextImpl(QString& txt) const
{
    return m_view->text(m_id, txt);
}

bool CacheViewProxy::tooltipTextImpl(QString& tooltipText) const
{
    return m_view->tooltipText(m_id, tooltipText);
}

} // end namespace Qi
