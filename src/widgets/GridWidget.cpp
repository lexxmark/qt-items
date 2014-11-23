#include "GridWidget.h"
#include "cache/space/CacheSpaceGrid.h"
#include "items/cache/ViewCacheSpace.h"
#include "core/ext/Ranges.h"
#include "core/ext/Layouts.h"
#include "SpaceWidget_p.h"
#include <QScrollBar>

namespace Qi
{

GridWidget::GridWidget(QWidget* parent)
    : QAbstractScrollArea(parent),
      m_isSubGridsLayoutValid(false)
{
    // initialize main grid
    m_mainGrid = QSharedPointer<SpaceGrid>::create();
    m_mainGrid->setDimensions(3, 3);
    auto cacheSpace = QSharedPointer<CacheSpaceGrid>::create(m_mainGrid);
    m_impl.reset(new SpaceWidgetPrivate(viewport(), cacheSpace));
    connect(cacheSpace.data(), &CacheSpace::cacheChanged, this, &GridWidget::onCacheSpaceChanged);

    //initialize sub rows and columns
    for (int i = 0; i < 3; ++i)
    {
        m_rows[i] = QSharedPointer<Lines>::create();
        m_columns[i] = QSharedPointer<Lines>::create();
    }

    //initialize sub grids and caches
    auto modelCache = QSharedPointer<ModelStorageGrid<QSharedPointer<CacheSpace>>>::create(m_mainGrid);
    for (ItemID subID = ItemID(0, 0); subID.row < 3; ++subID.row)
    {
        for (subID.column = 0; subID.column < 3; ++subID.column)
        {
            SpaceHints hints = (subID.row == 1) ? SpaceHintSimilarViewsByColumns : SpaceHintNone;
            auto subGrid = QSharedPointer<SpaceGrid>::create(m_rows[subID.row], m_columns[subID.column], hints);
            auto cacheSpace = QSharedPointer<CacheSpaceGrid>::create(subGrid);

            m_cacheSubGrids[subID.row][subID.column] = cacheSpace;
            modelCache->setValue(subID, cacheSpace);

            connect(subGrid.data(), &Space::spaceChanged, this, &GridWidget::onSubGridChanged);
            connect(cacheSpace.data(), &CacheSpace::cacheChanged, this, &GridWidget::onCacheSpaceChanged);
        }
    }

    // add sub-grid caches to main grid schema
    m_mainGrid->addSchema(makeRangeAll(), QSharedPointer<ViewCacheSpace>::create(modelCache), makeLayoutBackground());

    // enable tracking mouse moves
    viewport()->setMouseTracking(true);
}

GridWidget::~GridWidget()
{
}

const SpaceGrid& GridWidget::subGrid(const ItemID& subGridID) const
{
    return *m_cacheSubGrids[subGridID.row][subGridID.column]->spaceGrid();
}

SpaceGrid& GridWidget::subGrid(const ItemID& subGridID)
{
    return *m_cacheSubGrids[subGridID.row][subGridID.column]->spaceGrid();
}

CacheSpaceGrid& GridWidget::cacheSubGrid(const ItemID& subGridID)
{
    return *m_cacheSubGrids[subGridID.row][subGridID.column];
}

bool GridWidget::viewportEvent(QEvent* event)
{
    bool result = QAbstractScrollArea::viewportEvent(event);

    switch (event->type())
    {
    case QEvent::Paint:
        validateSubGridsLayout();
        break;

    case QEvent::Resize:
        updateScrollbars();
        invalidateSubGridsLayout();
        break;

    default:
        ;
    }

    result |= m_impl->ownerEvent(event);
    return result;
}

void GridWidget::scrollContentsBy(int dx, int dy)
{
    QAbstractScrollArea::scrollContentsBy(dx, dy);
    updateCacheScrollOffsets();
}

QSize GridWidget::viewportSizeHint() const
{
    // return sum size of all sub-grids
    return subGrid(topLeftID).size() + subGrid(clientID).size() + subGrid(bottomRightID).size();
}

void GridWidget::onSubGridChanged(const Space* space, ChangeReason reason)
{
    if (reason & ChangeReasonSpaceStructure)
    {
        invalidateSubGridsLayout();
        updateScrollbars();
        // request to recalculate layout
        updateGeometry();
    }
}

void GridWidget::onCacheSpaceChanged(const CacheSpace* cache, ChangeReason reason)
{
    // request to repaint widget
    update();
}

void GridWidget::updateScrollbars()
{
    // client visible area only
    QSize scrollableSize = viewport()->size() - subGrid(topLeftID).size() - subGrid(bottomRightID).size();

    // no space for client grid
    if (scrollableSize.width() <= 0 || scrollableSize.height() <= 0)
    {
        // don't show scrollbars
        verticalScrollBar()->setSingleStep(0);
        verticalScrollBar()->setPageStep(0);
        verticalScrollBar()->setRange(0, 0);
        horizontalScrollBar()->setSingleStep(0);
        horizontalScrollBar()->setPageStep(0);
        horizontalScrollBar()->setRange(0, 0);
    }
    else
    {
        QSize virtualSize = subGrid(clientID).size();
        // update vertical scrollbar
        verticalScrollBar()->setSingleStep(scrollableSize.height() / 10);
        verticalScrollBar()->setPageStep(scrollableSize.height());
        verticalScrollBar()->setRange(0, qMax(0, virtualSize.height() - scrollableSize.height() + 2));
        // update horizontal scrollbar
        horizontalScrollBar()->setSingleStep(scrollableSize.width() / 10);
        horizontalScrollBar()->setPageStep(scrollableSize.width());
        horizontalScrollBar()->setRange(0, qMax(0, virtualSize.width() - scrollableSize.width() + 2));
    }
}

void GridWidget::updateCacheScrollOffsets()
{
    // update scroll positions
    QPoint scrollPos(horizontalScrollBar()->value(), verticalScrollBar()->value());

    // non scrollable sub-grids (corner sub-grids)
    QPoint fixedScrollPos(0, 0);
    cacheSubGrid(topLeftID).setScrollOffset(fixedScrollPos);
    cacheSubGrid(topRightID).setScrollOffset(fixedScrollPos);
    cacheSubGrid(bottomLeftID).setScrollOffset(fixedScrollPos);
    cacheSubGrid(bottomRightID).setScrollOffset(fixedScrollPos);

    // horizontally scrollable sub-grids (top and bottom)
    QPoint horScrollPos(scrollPos.x(), 0);
    cacheSubGrid(topID).setScrollOffset(horScrollPos);
    cacheSubGrid(bottomID).setScrollOffset(horScrollPos);

    // vertically scrollable sub-grids (left and right)
    QPoint verScrollPos(0, scrollPos.y());
    cacheSubGrid(leftID).setScrollOffset(verScrollPos);
    cacheSubGrid(rightID).setScrollOffset(verScrollPos);

    // fully scrollable sub-grid - client
    cacheSubGrid(clientID).setScrollOffset(scrollPos);
}

void GridWidget::invalidateSubGridsLayout()
{
    m_isSubGridsLayoutValid = false;
    // request to repaint
    update();
}

void GridWidget::validateSubGridsLayout()
{
    if (m_isSubGridsLayoutValid)
        return;

    {
        QSize visibleSize = viewport()->size();

        QSize topLeftSubGridSize = subGrid(topLeftID).size();
        // adjust to visible size
        topLeftSubGridSize.rwidth() = qMin(topLeftSubGridSize.width(), visibleSize.width());
        topLeftSubGridSize.rheight() = qMin(topLeftSubGridSize.height(), visibleSize.height());
        // decrease visible size
        visibleSize = visibleSize - topLeftSubGridSize;

        QSize bottomRightSubGridSize = subGrid(bottomRightID).size();
        // adjust to visible size
        bottomRightSubGridSize.rwidth() = qMin(bottomRightSubGridSize.width(), visibleSize.width());
        bottomRightSubGridSize.rheight() = qMin(bottomRightSubGridSize.height(), visibleSize.height());
        // decrease total size
        visibleSize = visibleSize - bottomRightSubGridSize;

        // remain visible size becomes client size
        QSize clientSize = visibleSize;

        // update super-grid sizes
        // sub-grids caches will update it's windows according to this sizes
        auto rows = m_mainGrid->rows();
        rows->setLineSize(0, topLeftSubGridSize.height());
        rows->setLineSize(1, clientSize.height());
        rows->setLineSize(2, bottomRightSubGridSize.height());
        auto columns = m_mainGrid->columns();
        columns->setLineSize(0, topLeftSubGridSize.width());
        columns->setLineSize(1, clientSize.width());
        columns->setLineSize(2, bottomRightSubGridSize.width());

        updateCacheScrollOffsets();
    }

    m_isSubGridsLayoutValid = true;

    // continue handling controllers
    m_impl->resumeControllers();
}


} // end namespace Qi
