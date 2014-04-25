#ifndef QI_CACHE_H
#define QI_CACHE_H

#include "CellsSchema.h"
#include <QVector>

namespace Qi
{

class QI_EXPORT CacheView
{
public:
    CacheView();
    CacheView(const ViewSchema& schema);
    CacheView(const CacheView& other);
    ~CacheView();
    
    CacheView& operator=(const CacheView& other);

    // initializes m_rect according to m_layout
    void doLayout(const QWidget* widget, const CellID& cell, QRect& availableRect) const;
    // expands size according to m_layout
    void doExpandSize(const QWidget* widget, const CellID& cell, QSize& size) const;
    // draws m_view within m_rects
    void draw(QPainter* painter, const QWidget* widget, const CellID& cell) const;

protected:
    ViewSchema m_schema;
    mutable QRect m_rect;
};

class QI_EXPORT CacheCell
{
public:
    CacheCell();
    explicit CacheCell(CellID cell);
    CacheCell(const CacheCell& other);

    CacheCell& operator=(const CacheCell& other);
    void swap(CacheCell& other);

    const CellID& cell() const { return m_cell; }
    void setCell(const CellID& cell);
    
    void reinit(const QVector<ViewSchema>& views, QRect cellRect);
    void clear();
    
    void draw(QPainter* painter, const QWidget* widget) const;
    QSize sizeHint(const QWidget* widget) const;
    
protected:
    CellID m_cell;
    QRect m_rect;
    QVector<CacheView> m_cacheViews;
    mutable bool m_isLayoutValid;
};

} // end namespace Qi

#endif // QI_CACHE_H
