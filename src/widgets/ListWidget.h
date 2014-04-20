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
    explicit ListWidget(const QSharedPointer<Grid>& grid, QWidget *parent = nullptr);
    virtual ~ListWidget();

    Grid& grid();
    const Grid& grid() const;

    void setGrid(const QSharedPointer<Grid>& grid);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void scrollContentsBy(int dx, int dy) override;

private:
    void init();
    void updateFrame();

    QScopedPointer<ItemWidgetPrivate> d;

    QSharedPointer<Grid> m_grid;
    QSharedPointer<ViewGrid> m_viewGrid;
};

} // end namespace Qi

#endif // QI_LIST_WIDGET_H
