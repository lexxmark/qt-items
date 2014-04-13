#include "Cache.h"
#include "ViewInfo.h"

//#define DEBUG_RECTS

namespace Qi
{

CacheView::CacheView(const View* view, const Layout *layout)
    : m_view(view),
      m_layout(layout)
{
}

CacheView::CacheView(CacheView&& other)
    : m_view(nullptr),
      m_layout(nullptr)
{
    std::swap(m_view, other.m_view);
    std::swap(m_layout, other.m_layout);
    std::swap(m_rect, other.m_rect);
}

CacheView::~CacheView()
{
}

CacheView& CacheView::operator=(CacheView&& other)
{
    std::swap(m_view, other.m_view);
    std::swap(m_layout, other.m_layout);
    std::swap(m_rect, other.m_rect);
    return *this;
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
    : m_cell(cell),
      m_isLayoutValid(false)
{
}

void CacheCell::reinit(const std::vector<ViewInfo>& views, QRect cellRect)
{
    std::vector<CacheView> cacheViews;
    cacheViews.reserve(views.size());
    
    for (const auto& viewInfo: views)
    {
        cacheViews.emplace_back(CacheView(viewInfo.view.data(), viewInfo.layout.data()));
    }
    
    m_rect = cellRect;
    std::swap(m_cacheViews, cacheViews);
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
            cacheView.doLayout(dc, m_cell, availableRect);
        }
        m_isLayoutValid = true;
    }

    for (const auto& cacheView: m_cacheViews)
    {
        cacheView.draw(dc, m_cell);
    }
}

QSize CacheCell::sizeHint(DrawContext& dc) const
{
    QSize size(0, 0);
    
    // accumulate size in reverse order
    for (auto rit = m_cacheViews.rbegin(); rit != m_cacheViews.rend(); ++rit)
    {
        (*rit).doExpandSize(dc, m_cell, size);
    }
    
    return size;
}

} // end namespace Qi
