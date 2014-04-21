#ifndef QI_CACHE_H
#define QI_CACHE_H

#include "Layout.h"
#include "Range.h"
#include <vector>

namespace Qi
{

struct ViewSchema;

class QI_EXPORT CacheView
{
    Q_DISABLE_COPY(CacheView)

public:
    CacheView(const View* view, const Layout* layout);
    CacheView(CacheView&& other);
    ~CacheView();
    
    CacheView& operator=(CacheView&& other);

    // initializes m_rect according to m_layout
    void doLayout(const QWidget* widget, const CellID& cell, QRect& availableRect) const;
    // expands size according to m_layout
    void doExpandSize(const QWidget* widget, const CellID& cell, QSize& size) const;
    // draws m_view within m_rects
    void draw(QPainter* painter, const QWidget* widget, const CellID& cell) const;

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
    CacheCell(CacheCell&& other);

    CacheCell& operator=(CacheCell&& other);
    void swap(CacheCell& other);

    const CellID& cell() const { return m_cell; }
    void setCell(const CellID& cell);
    
    void reinit(const std::vector<ViewSchema>& views, QRect cellRect);
    
    void draw(QPainter* painter, const QWidget* widget) const;
    QSize sizeHint(const QWidget* widget) const;
    
protected:
    CellID m_cell;
    QRect m_rect;
    std::vector<CacheView> m_cacheViews;
    mutable bool m_isLayoutValid;
};

} // end namespace Qi

#endif // QI_CACHE_H
