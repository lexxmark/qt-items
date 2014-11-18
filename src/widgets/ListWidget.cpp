#include "ListWidget.h"
//#include "ItemWidget_p.h"

/*
#include "items/cache/ViewCache.h"
#include "core/Layouts.h"
#include <QPainter>
#include <QResizeEvent>
#include <QScrollBar>

namespace Qi
{

ListWidget::ListWidget(QWidget *parent)
    : QAbstractScrollArea(parent),
      m_grid(new SpaceGrid())
{
    init();
}

ListWidget::ListWidget(SpaceGrid* grid, QWidget *parent)
    : QAbstractScrollArea(parent),
      m_grid(grid)
{
    init();
}

ListWidget::~ListWidget()
{
    Q_ASSERT(viewport());
    // grid should exist
    Q_ASSERT(!m_grid.isNull());
}

SpaceGrid& ListWidget::grid()
{
    return *m_grid;
}

const SpaceGrid& ListWidget::grid() const
{
    return *m_grid;
}

void ListWidget::setGrid(SpaceGrid* grid)
{
    m_grid = grid;
    if (!m_grid->parent())
        m_grid->setParent(this);

    m_viewGrid->setGrid(m_grid);
}

bool ListWidget::viewportEvent(QEvent* event)
{
    bool result = QAbstractScrollArea::viewportEvent(event);
    d->event(event);
    return result;
}

void ListWidget::resizeEvent(QResizeEvent* event)
{
    // update scrollbars
    QSize viewportSize = viewport()->size();
    QSize virtualSize = d->sizeHint();

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

    if (!m_grid->parent())
        m_grid->setParent(this);

    m_viewGrid = new ViewCache(m_grid);

    d.reset(new ItemWidgetPrivate(viewport()));
    d->addViewSchema(new LayoutAll(), m_viewGrid.data());

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
*/
