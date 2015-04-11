/*
   Copyright (c) 2008-1015 Alex Zhondin <qtinuum.team@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "GridWidget.h"
#include "cache/space/CacheSpaceGrid.h"
#include "cache/CacheItem.h"
#include "items/cache/ViewCacheSpace.h"
#include "core/ext/Ranges.h"
#include "core/ext/Layouts.h"
#include <QScrollBar>

namespace Qi
{

GridWidget::GridWidget(QWidget* parent)
    : SpaceWidgetScrollAbstract(parent)
{
    // initialize main grid
    m_mainGrid = QSharedPointer<SpaceGrid>::create();
    m_mainGrid->setDimensions(3, 3);
    auto mainCacheSpace = QSharedPointer<CacheSpaceGrid>::create(m_mainGrid);

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
            SpaceGridHint hint = (subID.row == 1) ? SpaceGridHintSameSchemasByColumn : SpaceGridHintNone;
            auto subGrid = QSharedPointer<SpaceGrid>::create(m_rows[subID.row], m_columns[subID.column], hint);
            auto cacheSpace = QSharedPointer<CacheSpaceGrid>::create(subGrid);

            m_cacheSubGrids[subID.row][subID.column] = cacheSpace;
            modelCache->setValue(subID, cacheSpace);

            connect(subGrid.data(), &Space::spaceChanged, this, &GridWidget::onSubGridChanged);
            connect(cacheSpace.data(), &CacheSpace::cacheChanged, this, &GridWidget::onCacheSpaceChanged);
        }
    }

    // add sub-grid caches to main grid schema
    m_mainGrid->addSchema(makeRangeAll(), QSharedPointer<ViewCacheSpace>::create(modelCache), makeLayoutBackground());

    initSpaceWidgetScrollable(mainCacheSpace, cacheSubGrid());
}

GridWidget::~GridWidget()
{
}

const QSharedPointer<SpaceGrid>& GridWidget::subGrid(const ItemID& subGridID) const
{
    return m_cacheSubGrids[subGridID.row][subGridID.column]->spaceGrid();
}

const QSharedPointer<CacheSpaceGrid>& GridWidget::cacheSubGrid(const ItemID& subGridID) const
{
    return m_cacheSubGrids[subGridID.row][subGridID.column];
}

void GridWidget::ensureVisibleImpl(const ItemID& visibleItem, const CacheSpace *cacheSpace, bool validateItem)
{
    auto cacheSpaceGrid = qobject_cast<const CacheSpaceGrid*>(cacheSpace);
    if (!cacheSpaceGrid)
    {
        // cacheSpace is invalid
        Q_ASSERT(false);
        return;
    }

    // calculate scroll size and position if needed
    validateCacheItemsLayout();

    QRect clientGridRect = m_mainGrid->itemRect(clientID);
    if (clientGridRect.isEmpty())
        return;

    QPoint scrollPos(horizontalScrollBar()->value(), verticalScrollBar()->value());
    QRect itemRect = cacheSpace->space().itemRect(visibleItem);

    // if client columns -> correct scroll position horizontally
    if (cacheSpaceGrid->spaceGrid()->columns() ==  m_columns[1])
    {
        if ((itemRect.width() >= clientGridRect.width()) || (itemRect.left() < scrollPos.x()))
            scrollPos.rx() = itemRect.left();
        else if (itemRect.right() > (scrollPos.x() + clientGridRect.width()))
            scrollPos.rx() = itemRect.right() - clientGridRect.width();
    }

    // if client rows -> correct scroll position vertically
    if (cacheSpaceGrid->spaceGrid()->rows() ==  m_rows[1])
    {
        if ((itemRect.height() >= clientGridRect.height()) || (itemRect.top() < scrollPos.y()))
            scrollPos.ry() = itemRect.top();
        else if (itemRect.bottom() > (scrollPos.y() + clientGridRect.height()))
            scrollPos.ry() = itemRect.bottom() - clientGridRect.height();
    }

    // scroll to new position
    horizontalScrollBar()->setValue(scrollPos.x());
    verticalScrollBar()->setValue(scrollPos.y());

    if (validateItem)
    {
        validateCacheItemsLayout();

        // validate cache views
        CacheItem* cacheItem = const_cast<CacheItem*>(cacheSpace->cacheItem(visibleItem));
        if (!cacheItem)
            return;

        cacheItem->validateCacheView(guiContext(), &cacheSpace->window());
    }
}

QSize GridWidget::viewportSizeHint() const
{
    // return sum size of all sub-grids
    return subGrid(topLeftID)->size() + subGrid(clientID)->size() + subGrid(bottomRightID)->size();
}

void GridWidget::onSubGridChanged(const Space* /*space*/, ChangeReason reason)
{
    if (reason & ChangeReasonSpaceStructure)
    {
        invalidateCacheItemsLayout();
        updateScrollbars();
        // request to recalculate layout
        updateGeometry();
    }
}

void GridWidget::onCacheSpaceChanged(const CacheSpace* /*cache*/, ChangeReason /*reason*/)
{
    // repaint widget
    viewport()->update();
}

QSize GridWidget::calculateVirtualSizeImpl() const
{
    return subGrid(clientID)->size();
}

QSize GridWidget::calculateScrollableSizeImpl() const
{
    return viewport()->size() - subGrid(topLeftID)->size() - subGrid(bottomRightID)->size();
}

void GridWidget::updateCacheScrollOffsetImpl()
{
    // get scroll positions
    QPoint scrollPos(horizontalScrollBar()->value(), verticalScrollBar()->value());

    // non scrollable sub-grids (corner sub-grids)
    QPoint fixedScrollPos(0, 0);
    cacheSubGrid(topLeftID)->setScrollOffset(fixedScrollPos);
    cacheSubGrid(topRightID)->setScrollOffset(fixedScrollPos);
    cacheSubGrid(bottomLeftID)->setScrollOffset(fixedScrollPos);
    cacheSubGrid(bottomRightID)->setScrollOffset(fixedScrollPos);

    // horizontally scrollable sub-grids (top and bottom)
    QPoint horScrollPos(scrollPos.x(), 0);
    cacheSubGrid(topID)->setScrollOffset(horScrollPos);
    cacheSubGrid(bottomID)->setScrollOffset(horScrollPos);

    // vertically scrollable sub-grids (left and right)
    QPoint verScrollPos(0, scrollPos.y());
    cacheSubGrid(leftID)->setScrollOffset(verScrollPos);
    cacheSubGrid(rightID)->setScrollOffset(verScrollPos);

    // fully scrollable sub-grid - client
    cacheSubGrid(clientID)->setScrollOffset(scrollPos);
}

void GridWidget::validateCacheItemsLayoutImpl()
{
    QSize visibleSize = viewport()->size();

    QSize topLeftSubGridSize = subGrid(topLeftID)->size();
    // adjust to visible size
    topLeftSubGridSize.rwidth() = qMin(topLeftSubGridSize.width(), visibleSize.width());
    topLeftSubGridSize.rheight() = qMin(topLeftSubGridSize.height(), visibleSize.height());
    // decrease visible size
    visibleSize = visibleSize - topLeftSubGridSize;

    QSize bottomRightSubGridSize = subGrid(bottomRightID)->size();
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
}


} // end namespace Qi
