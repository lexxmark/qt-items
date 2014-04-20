#include "Cache.h"
#include "CellsSchema.h"

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

void CacheView::doLayout(const QWidget* widget, const CellID& cell, QRect& availableRect) const
{
    m_rect = QRect(0, 0, 0, 0);
    m_layout->doLayout(m_view, cell, widget, availableRect, m_rect);
}

void CacheView::doExpandSize(const QWidget* widget, const CellID& cell, QSize& size) const
{
    m_layout->doExpandSize(m_view, cell, widget, size);
}

void CacheView::draw(QPainter* painter, const QWidget* widget, const CellID& cell) const
{
#ifdef DEBUG_RECTS
    painter->drawRect(m_rect);
#endif
    
    m_view->draw(painter, widget, cell, m_rect);
}

CacheCell::CacheCell(CellID cell)
    : m_cell(cell),
      m_isLayoutValid(false)
{
}

CacheCell::CacheCell(CacheCell&& other)
    : m_isLayoutValid(false)
{
    swap(other);
}

CacheCell& CacheCell::operator=(CacheCell&& other)
{
    swap(other);
    return *this;
}

void CacheCell::swap(CacheCell& other)
{
    std::swap(m_cell, other.m_cell);
    std::swap(m_rect, other.m_rect);
    std::swap(m_cacheViews, other.m_cacheViews);
    std::swap(m_isLayoutValid, other.m_isLayoutValid);
}

void CacheCell::reinit(const std::vector<ViewSchema>& views, QRect cellRect)
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

void CacheCell::draw(QPainter* painter, const QWidget* widget) const
{
#ifdef DEBUG_RECTS
    painter->drawRect(m_rect);
#endif
    
    if (!m_isLayoutValid)
    {
        QRect availableRect = m_rect;
        for (const auto& cacheView: m_cacheViews)
        {
            cacheView.doLayout(widget, m_cell, availableRect);
        }
        m_isLayoutValid = true;
    }

    for (const auto& cacheView: m_cacheViews)
    {
        cacheView.draw(painter, widget, m_cell);
    }
}

QSize CacheCell::sizeHint(const QWidget* widget) const
{
    if (m_cacheViews.empty())
        return QSize();

    QSize size(0, 0);
    
    // accumulate size in reverse order
    for (auto rit = m_cacheViews.rbegin(); rit != m_cacheViews.rend(); ++rit)
    {
        (*rit).doExpandSize(widget, m_cell, size);
    }
    
    return size;
}

} // end namespace Qi
