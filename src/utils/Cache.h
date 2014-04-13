#ifndef QI_CACHE_H
#define QI_CACHE_H

#include "Layout.h"
#include "Range.h"
#include <vector>

namespace Qi
{

struct ViewInfo;

class QI_EXPORT CacheView
{
    Q_DISABLE_COPY(CacheView)

public:
    //CacheView();
    CacheView(const View* view, const Layout* layout);
    CacheView(CacheView&& other);
    ~CacheView();
    
    // initializes m_rect according to m_layout
    void doLayout(DrawContext& dc, const CellID& cell, QRect& availableRect) const;
    // expands size according to m_layout
    void doExpandSize(DrawContext& dc, const CellID& cell, QSize& size) const;
    // draws m_view within m_rects
    void draw(DrawContext& dc, const CellID& cell) const;

    CacheView& operator=(CacheView&& other);

protected:
    const View* m_view;
    const Layout* m_layout;
    mutable QRect m_rect;
};

class QI_EXPORT CacheCell
{
    Q_DISABLE_COPY(CacheCell)

public:
    explicit CacheCell(CellID cell);

    CacheCell(CacheCell&& other)
        : m_isLayoutValid(false)
    {
        swap(other);
    }

    CacheCell& operator=(CacheCell&& other)
    {
        swap(other);
        return *this;
    }
    
    const CellID& cell() const { return m_cell; }
    
    void reinit(const std::vector<ViewInfo>& views, QRect cellRect);
    
    void draw(DrawContext& dc) const;
    QSize sizeHint(DrawContext& dc) const;
    
    void swap(CacheCell& other)
    {
        std::swap(m_cell, other.m_cell);
        std::swap(m_rect, other.m_rect);
        std::swap(m_cacheViews, other.m_cacheViews);
        std::swap(m_isLayoutValid, other.m_isLayoutValid);
    }
    
protected:
    CellID m_cell;
    QRect m_rect;
    std::vector<CacheView> m_cacheViews;
    mutable bool m_isLayoutValid;
};

} // end namespace Qi

#endif // QI_CACHE_H
