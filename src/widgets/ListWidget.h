#ifndef QI_LIST_WIDGET_H
#define QI_LIST_WIDGET_H

#include "QiAPI.h"
#include "space/SpaceGrid.h"
#include <QAbstractScrollArea>

namespace Qi
{

class ItemWidgetPrivate;
class ViewCache;
/*
class QI_EXPORT ListWidget: public QAbstractScrollArea
{
    Q_OBJECT

public:
    explicit ListWidget(QWidget *parent = nullptr);
    explicit ListWidget(SpaceGrid* grid, QWidget *parent = nullptr);
    virtual ~ListWidget();

    SpaceGrid& grid();
    const SpaceGrid& grid() const;

    void setGrid(SpaceGrid* grid);

protected:
    bool viewportEvent(QEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void scrollContentsBy(int dx, int dy) override;

private:
    void init();
    void updateFrame();

    QScopedPointer<ItemWidgetPrivate> d;

    QPointer<SpaceGrid> m_grid;
    QPointer<ViewCache> m_viewGrid;
};
*/
} // end namespace Qi

#endif // QI_LIST_WIDGET_H
