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

CacheView::CacheView()
    : m_layout(nullptr),
      m_view(nullptr),
      m_showTooltip(false)
{
    // this constructor is required for QVector
    Q_ASSERT(false);
}

CacheView::CacheView(const Layout *layout, const View *view, const QRect &rect)
    : m_layout(layout),
      m_view(view),
      m_rect(rect),
      m_showTooltip(false)
{
    Q_ASSERT(m_layout);
    Q_ASSERT(m_view);
}

CacheView::CacheView(const CacheView& other)
    : m_layout(other.m_layout),
      m_view(other.m_view),
      m_rect(other.m_rect),
      m_showTooltip(other.m_showTooltip),
      m_subViews(other.m_subViews)
{
}

CacheView::~CacheView()
{
}

CacheView& CacheView::operator=(const CacheView& other)
{
    m_layout = other.m_layout;
    m_view =other.m_view;
    m_rect = other.m_rect;
    m_showTooltip = other.m_showTooltip;
    m_subViews= other.m_subViews;
    return *this;
}

void CacheView::draw(QPainter* painter, const GuiContext &ctx, const ItemID& item, const QRect& itemRect, const QRect *visibleRect) const
{
    if (drawProxy)
        drawProxy(this, painter, ctx, item, itemRect, visibleRect);
    else
        drawRaw(painter, ctx, item, itemRect, visibleRect);
}

void CacheView::drawRaw(QPainter* painter, const GuiContext &ctx, const ItemID& item, const QRect& itemRect, const QRect *visibleRect) const
{
#ifdef DEBUG_RECTS
    painter->save();
    painter->setPen(Qt::blue);
    painter->drawRect(m_rect);
    painter->restore();
#endif
    
    m_showTooltip = false;

    m_view->draw(painter, ctx, CacheContext(item, itemRect, *this, visibleRect), &m_showTooltip);

    // exclude rect of float views because they can overlap other views
    if (m_layout->isFloat())
    {
        //*painter->ExcludeClipRect(&rect);
    }
}

void CacheView::cleanupDraw(QPainter* painter, const GuiContext &ctx, const ItemID& item, const QRect& itemRect, const QRect* visibleRect) const
{
    m_view->cleanupDraw(painter, ctx, CacheContext(item, itemRect, *this, visibleRect));
}

bool CacheView::tooltipText(const ItemID& item, QString& tooltipText) const
{
    if (!m_showTooltip)
        return false;

    return m_view->tooltipText(item, tooltipText);
}

} // end namespace Qi
