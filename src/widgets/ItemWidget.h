#ifndef QI_ITEM_WIDGET_H
#define QI_ITEM_WIDGET_H

#include "../utils/Cache.h"
#include "../utils/CellsSchema.h"
#include <QWidget>

namespace Qi
{

class ItemWidgetPrivate;

class QI_EXPORT ItemWidget: public QWidget
{
    Q_OBJECT

public:
    explicit ItemWidget(QWidget *parent = nullptr);
    virtual ~ItemWidget();
    
    void addViewSchema(QSharedPointer<View> view, QSharedPointer<Layout> layout);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    
protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent * event) override;
    
private:
    QScopedPointer<ItemWidgetPrivate> d;
};

} // end namespace Qi

#endif // QI_ITEM_WIDGET_H
