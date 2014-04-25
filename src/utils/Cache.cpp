#include "Cache.h"
#include "CellsSchema.h"

//#define DEBUG_RECTS

namespace Qi
{

CacheView::CacheView()
{
}

CacheView::CacheView(const ViewSchema& schema)
    : m_schema(schema)
{
}

CacheView::CacheView(const CacheView& other)
    : m_schema(other.m_schema),
      m_rect(other.m_rect)
{
}

CacheView::~CacheView()
{
}

CacheView& CacheView::operator=(const CacheView& other)
{
    m_schema = other.m_schema;
    m_rect = other.m_rect;
    return *this;
}

void CacheView::doLayout(const QWidget* widget, const CellID& cell, QRect& availableRect) const
{
    m_rect = QRect(0, 0, 0, 0);
    m_schema.layout->doLayout(m_schema.view, cell, widget, availableRect, m_rect);
}

void CacheView::doExpandSize(const QWidget* widget, const CellID& cell, QSize& size) const
{
    m_schema.layout->doExpandSize(m_schema.view, cell, widget, size);
}

void CacheView::draw(QPainter* painter, const QWidget* widget, const CellID& cell) const
{
#ifdef DEBUG_RECTS
    painter->save();
    painter->setPen(Qt::blue);
    painter->drawRect(m_rect);
    painter->restore();
#endif
    
    m_schema.view->draw(painter, widget, cell, m_rect);
}

CacheCell::CacheCell()
{
    // should not be used
    Q_ASSERT(false);
}

CacheCell::CacheCell(CellID cell)
    : m_cell(cell),
      m_isLayoutValid(false)
{
}

CacheCell::CacheCell(const CacheCell& other)
    : m_cell(other.m_cell),
      m_rect(other.m_rect),
      m_cacheViews(other.m_cacheViews),
      m_isLayoutValid(other.m_isLayoutValid)
{
}

CacheCell& CacheCell::operator=(const CacheCell& other)
{
    m_cell = other.m_cell;
    m_rect = other.m_rect;
    m_cacheViews = other.m_cacheViews;
    m_isLayoutValid = other.m_isLayoutValid;

    return *this;
}

void CacheCell::swap(CacheCell& other)
{
    std::swap(m_cell, other.m_cell);
    std::swap(m_rect, other.m_rect);
    std::swap(m_cacheViews, other.m_cacheViews);
    std::swap(m_isLayoutValid, other.m_isLayoutValid);
}

void CacheCell::setCell(const CellID& cell)
{
    if (m_cell == cell)
        return;

    m_cell = cell;
    m_cacheViews.clear();
    m_isLayoutValid = false;
}

void CacheCell::reinit(const QVector<ViewSchema>& views, QRect cellRect)
{
    QVector<CacheView> cacheViews;
    cacheViews.reserve(views.size());
    
    for (const auto& viewSchema: views)
    {
        cacheViews.append(CacheView(viewSchema));
    }
    
    m_rect = cellRect;
    std::swap(m_cacheViews, cacheViews);
    m_isLayoutValid = false;
}

void CacheCell::clear()
{
    m_cacheViews.clear();
    m_rect = QRect();
    m_isLayoutValid = false;
}

void CacheCell::draw(QPainter* painter, const QWidget* widget) const
{
#ifdef DEBUG_RECTS
    painter->save();
    painter->setPen(Qt::red);
    painter->drawRect(m_rect);
    painter->restore();
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
    auto it = m_cacheViews.end();
    while (it != m_cacheViews.begin())
    {
        --it;
        (*it).doExpandSize(widget, m_cell, size);
    }
    
    return size;
}

} // end namespace Qi
