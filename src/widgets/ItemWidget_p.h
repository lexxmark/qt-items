#ifndef QI_ITEM_WIDGET_P_H
#define QI_ITEM_WIDGET_P_H

#include "../utils/Cache.h"
#include "../utils/CellsSchema.h"

class QWidget;

namespace Qi
{

class ItemWidgetPrivate
{
    Q_DISABLE_COPY(ItemWidgetPrivate)

public:
    explicit ItemWidgetPrivate(QWidget* owner);
    ~ItemWidgetPrivate();
    
    void addViewSchema(Layout* layout, View* view, ControllerMouse* controller = nullptr);
    void clearViewSchemas();
    void setCell(const CellID& cell);

    QSize sizeHint() const;
    void event(QEvent* event);

private:
    void doPaintEvent(QPaintEvent* event);
    void doResizeEvent(QResizeEvent * event);
    
    void validateCacheCell() const;
    
    QWidget* m_owner;

    QVector<ViewSchema> m_views;

    mutable CacheCell m_cache;
    mutable bool m_isCacheValid;
    mutable QSize m_sizeHint;
};

} // end namespace Qi

#endif // QI_ITEM_WIDGET_P_H
