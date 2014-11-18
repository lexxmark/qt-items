#ifndef QI_GRID_WIDGET_H
#define QI_GRID_WIDGET_H

#include "QiAPI.h"
#include <QWidget>

namespace Qi
{

class QI_EXPORT GridWidget: public QWidget
{
    Q_OBJECT

public:
    explicit GridWidget(QWidget *parent = nullptr);
    virtual ~GridWidget();

protected:
    void paintEvent(QPaintEvent* event) override;
};

} // end namespace Qi

#endif // QI_GRID_WIDGET_H
