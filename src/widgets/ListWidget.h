#ifndef QI_LIST_WIDGET_H
#define QI_LIST_WIDGET_H

#include "../QiAPI.h"
#include <QWidget>

namespace Qi
{

class QI_EXPORT ListWidget: public QWidget
{
    Q_OBJECT

public:
    ListWidget(QWidget *parent = nullptr);
    virtual ~ListWidget();

protected:
    void paintEvent(QPaintEvent* event) override;
};

} // end namespace Qi

#endif // QI_LIST_WIDGET_H
