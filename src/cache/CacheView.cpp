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

#include "CacheView.h"
#include "core/Layout.h"
#include "core/View.h"

//#define DEBUG_RECTS

namespace Qi
{

CacheView2::CacheView2()
    : m_view(nullptr),
      m_showTooltip(false)
{
    // this constructor is required for QVector
    Q_ASSERT(false);
}

CacheView2::CacheView2(const View *view, const QRect &rect)
    : m_view(view),
      m_rect(rect),
      m_showTooltip(false)
{
    Q_ASSERT(m_view);
}

CacheView2::CacheView2(const CacheView2& other)
    : m_view(other.m_view),
      m_rect(other.m_rect),
      m_showTooltip(other.m_showTooltip),
      m_subViews(other.m_subViews)
{
}

CacheView2::~CacheView2()
{
}

CacheView2& CacheView2::operator=(const CacheView2& other)
{
    m_view =other.m_view;
    m_rect = other.m_rect;
    m_showTooltip = other.m_showTooltip;
    m_subViews= other.m_subViews;
    return *this;
}

void CacheView2::draw(QPainter* painter, const GuiContext &ctx, ID id, const QRect& itemRect, const QRect *visibleRect) const
{
    if (drawProxy)
        drawProxy(this, painter, ctx, id, itemRect, visibleRect);
    else
        drawRaw(painter, ctx, id, itemRect, visibleRect);
}

void CacheView2::drawRaw(QPainter* painter, const GuiContext &ctx, ID id, const QRect& itemRect, const QRect *visibleRect) const
{
#ifdef DEBUG_RECTS
    painter->save();
    painter->setPen(Qt::blue);
    painter->drawRect(m_rect);
    painter->restore();
#endif
    
    m_showTooltip = false;

    m_view->draw(painter, ctx, CacheContext(id, itemRect, *this, visibleRect), &m_showTooltip);

    // exclude rect of float views because they can overlap other views
//    if (m_layout->isFloat())
//    {
//        //*painter->ExcludeClipRect(&rect);
//    }
}

void CacheView2::cleanupDraw(QPainter* painter, const GuiContext &ctx, ID id, const QRect& itemRect, const QRect* visibleRect) const
{
    m_view->cleanupDraw(painter, ctx, CacheContext(id, itemRect, *this, visibleRect));
}

bool CacheView2::tooltipText(ID id, QString& tooltipText) const
{
    if (!m_showTooltip)
        return false;

    return m_view->tooltipText(id, tooltipText);
}

CacheView::CacheView(const CacheView* parent, QRect rect)
    : m_parent(parent),
      m_rect(rect)
{}

void CacheView::emitCacheViewChanged(ChangeReason reason)
{
    emit cacheViewChanged(this, reason);
}

void CacheView::setRect(QRect rect)
{
    if (m_rect == rect)
        return;

    m_rect = rect;
    emitCacheViewChanged(ChangeReasonViewSize);
}

} // end namespace Qi
