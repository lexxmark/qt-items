#ifndef QI_LIST_WIDGET_H
#define QI_LIST_WIDGET_H

#include "../QiAPI.h"
#include "../utils/Grid.h"
#include <QAbstractScrollArea>

namespace Qi
{

class ItemWidgetPrivate;
class ViewGrid;

class QI_EXPORT ListWidget: public QAbstractScrollArea
{
    Q_OBJECT

public:
    explicit ListWidget(QWidget *parent = nullptr);
    explicit ListWidget(Grid* grid, QWidget *parent = nullptr);
    virtual ~ListWidget();

    Grid& grid();
    const Grid& grid() const;

    void setGrid(Grid* grid);

protected:
    bool viewportEvent(QEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void scrollContentsBy(int dx, int dy) override;

private:
    void init();
    void updateFrame();

    QScopedPointer<ItemWidgetPrivate> d;

    QPointer<Grid> m_grid;
    QPointer<ViewGrid> m_viewGrid;
};

} // end namespace Qi

#endif // QI_LIST_WIDGET_H
