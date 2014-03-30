#include "Cache.h"

//#define DEBUG_RECTS

namespace Qi
{

CacheView::CacheView()
    : m_view(nullptr),
      m_layout(nullptr)
{
}

CacheView::CacheView(const View* view, const Layout *layout)
    : m_view(view),
      m_layout(layout)
{
}

CacheView::CacheView(const CacheView& other)
    : m_view(other.m_view),
      m_layout(other.m_layout),
      m_rect(other.m_rect)
{
}

CacheView::~CacheView()
{
}

void CacheView::doLayout(DrawContext& dc, const CellID& cell, QRect& availableRect) const
{
    m_rect = QRect(0, 0, 0, 0);
    m_layout->doLayout(*m_view, cell, dc, availableRect, m_rect);
}

void CacheView::doExpandSize(DrawContext& dc, const CellID& cell, QSize& size) const
{
    m_layout->doExpandSize(*m_view, cell, dc, size);
}

void CacheView::draw(DrawContext& dc, const CellID& cell) const
{
#ifdef DEBUG_RECTS
    dc.painter->drawRect(m_rect);
#endif
    
    m_view->draw(dc, cell, m_rect);
}

CacheCell::CacheCell(CellID cell)
    : cell(cell),
      m_isLayoutValid(false)
{
}

void CacheCell::reinit(const QVector<ViewInfo>& views, QRect cellRect)
{
    QVector<CacheView> cacheViews;
    
    for (const auto& viewInfo: views)
    {
        cacheViews.append(CacheView(viewInfo.view.data(), viewInfo.layout.data()));
    }
    
    m_rect = cellRect;
    m_cacheViews.swap(cacheViews);
    m_isLayoutValid = false;
}

void CacheCell::draw(DrawContext& dc) const
{
#ifdef DEBUG_RECTS
    dc.painter->drawRect(m_rect);
#endif
    
    if (!m_isLayoutValid)
    {
        QRect availableRect = m_rect;
        for (const auto& cacheView: m_cacheViews)
        {
            cacheView.doLayout(dc, cell, availableRect);
        }
        m_isLayoutValid = true;
    }

    for (const auto& cacheView: m_cacheViews)
    {
        cacheView.draw(dc, cell);
    }
}

QSize CacheCell::sizeHint(DrawContext& dc) const
{
    QSize size(0, 0);
    
    QVectorIterator<CacheView> rit(m_cacheViews);
    rit.toBack();
    while (rit.hasPrevious())
    {
        rit.previous().doExpandSize(dc, cell, size);
    }
    
    return size;
}

} // end namespace Qi
