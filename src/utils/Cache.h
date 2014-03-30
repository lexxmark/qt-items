#ifndef QI_CACHE_H
#define QI_CACHE_H

#include "Layout.h"

namespace Qi
{

struct QI_EXPORT ViewInfo
{
    QSharedPointer<View> view;
    QSharedPointer<Layout> layout;
};

class QI_EXPORT CacheView
{
public:
    CacheView();
    CacheView(const View* view, const Layout* layout);
    CacheView(const CacheView& other);
    ~CacheView();
    
    void doLayout(DrawContext& dc, const CellID& cell, QRect& availableRect) const;
    void doExpandSize(DrawContext& dc, const CellID& cell, QSize& size) const;
    void draw(DrawContext& dc, const CellID& cell) const;

protected:
    const View* m_view;
    const Layout* m_layout;
    mutable QRect m_rect;
};

class QI_EXPORT CacheCell
{
public:
    CacheCell(CellID cell);
    
    const CellID cell;
    
    void reinit(const QVector<ViewInfo>& views, QRect cellRect);
    
    void draw(DrawContext& dc) const;
    QSize sizeHint(DrawContext& dc) const;
    
protected:
    QRect m_rect;
    QVector<CacheView> m_cacheViews;
    mutable bool m_isLayoutValid;
};

} // end namespace Qi

#endif // QI_CACHE_H
