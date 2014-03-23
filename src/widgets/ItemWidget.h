#ifndef QI_ITEM_WIDGET_H
#define QI_ITEM_WIDGET_H

#include "../QiAPI.h"
#include <QWidget>

namespace Qi
{

class QI_EXPORT ItemWidget: public QWidget
{
    Q_OBJECT

public:
    ItemWidget(QWidget *parent = nullptr);
    virtual ~ItemWidget();

protected:
    void paintEvent(QPaintEvent* event) override;
};

} // end namespace Qi

#endif // QI_ITEM_WIDGET_H
