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
    
    void addViewSchema(Layout* layout, View* view, ControllerMouse* controller = nullptr);
    void clearViewSchemas();
    void setCell(const CellID& cell);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    
protected:
    bool event(QEvent* event) override;
//    void paintEvent(QPaintEvent* event) override;
//    void resizeEvent(QResizeEvent * event) override;
    
private:
    QScopedPointer<ItemWidgetPrivate> d;
};

} // end namespace Qi

#endif // QI_ITEM_WIDGET_H
