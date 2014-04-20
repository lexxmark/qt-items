#include "ListWidget.h"
#include "ItemWidget_p.h"
#include "../utils/ViewGrid.h"
#include "../utils/LayoutBasic.h"
#include <QPainter>
#include <QResizeEvent>
#include <QScrollBar>

namespace Qi
{

ListWidget::ListWidget(QWidget *parent)
    : QAbstractScrollArea(parent),
      m_grid(new Grid())
{
    init();
}

ListWidget::ListWidget(const QSharedPointer<Grid>& grid, QWidget *parent)
    : QAbstractScrollArea(parent),
      m_grid(grid)
{
    init();
}

ListWidget::~ListWidget()
{
}

Grid& ListWidget::grid()
{
    return *m_grid;
}

const Grid& ListWidget::grid() const
{
    return *m_grid;
}

void ListWidget::setGrid(const QSharedPointer<Grid>& grid)
{
    m_grid = grid;
    m_viewGrid->setGrid(m_grid);
}

void ListWidget::paintEvent(QPaintEvent* event)
{
    d->doPaintEvent(event);
}

void ListWidget::resizeEvent(QResizeEvent* event)
{
    d->doResizeEvent(event);

    // update scrollbars
    QSize viewportSize = viewport()->size();
    QSize virtualSize = d->doSizeHint();

    verticalScrollBar()->setSingleStep(viewportSize.height() / 10);
    verticalScrollBar()->setPageStep(viewportSize.height());
    verticalScrollBar()->setRange(0, qMax(0, virtualSize.height() - viewportSize.height() + 2));

    horizontalScrollBar()->setSingleStep(viewportSize.width() / 10);
    horizontalScrollBar()->setPageStep(viewportSize.width());
    horizontalScrollBar()->setRange(0, qMax(0, virtualSize.width() - viewportSize.width() + 2));

    updateFrame();
}

void ListWidget::scrollContentsBy(int dx, int dy)
{
    if (dx != 0 || dy != 0)
    {
        updateFrame();
    }

    QAbstractScrollArea::scrollContentsBy(dx, dy);
}

void ListWidget::init()
{
    // should not be initialized yet
    Q_ASSERT(m_viewGrid.isNull());
    Q_ASSERT(d.isNull());
    // should be initialized
    Q_ASSERT(!m_grid.isNull());

    m_viewGrid.reset(new ViewGrid(m_grid));

    d.reset(new ItemWidgetPrivate(viewport()));
    d->addViewSchema(m_viewGrid, QSharedPointer<Layout>(new LayoutAll()));

    viewport()->setMouseTracking(true);
}

void ListWidget::updateFrame()
{
    QRect newFrame;
    newFrame.setTop(verticalScrollBar()->value());
    newFrame.setLeft(horizontalScrollBar()->value());
    newFrame.setSize(viewport()->size());

    m_viewGrid->setFrame(newFrame);
}

} // end namespace Qi
