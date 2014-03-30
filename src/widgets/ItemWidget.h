#ifndef QI_ITEM_WIDGET_H
#define QI_ITEM_WIDGET_H

#include "../utils/Cache.h"
#include <QWidget>

namespace Qi
{

class QI_EXPORT ItemWidget: public QWidget
{
    Q_OBJECT

public:
    ItemWidget(QWidget *parent = nullptr);
    virtual ~ItemWidget();
    
    void addView(QSharedPointer<View> view, QSharedPointer<Layout> layout);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    
protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent * event) override;
    
private:
    void validateCacheCell();
    
    QVector<ViewInfo> m_views;
    CacheCell m_cache;
    mutable QSize m_sizeHint;
    bool m_isCacheValid;
};

} // end namespace Qi

#endif // QI_ITEM_WIDGET_H
