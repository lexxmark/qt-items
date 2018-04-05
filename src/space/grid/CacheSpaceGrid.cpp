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

#include "CacheSpaceGrid.h"
#include "cache/CacheItem.h"
#include "utils/auto_value.h"

namespace Qi
{

CacheSpaceGrid::CacheSpaceGrid(SharedPtr<SpaceGrid> grid, ViewApplicationMask viewApplicationMask)
    : CacheSpace(grid, viewApplicationMask),
      m_grid(grid)
{
}

CacheSpaceGrid::~CacheSpaceGrid()
{
}

bool CacheSpaceGrid::isItemInFrame(GridID visibleId) const
{
    validateItemsCache();

    return visibleId.row >= m_idStart.row && visibleId.row <= m_idEnd.row &&
            visibleId.column >= m_idStart.column && visibleId.column <= m_idEnd.column;
}

bool CacheSpaceGrid::isItemInFrameStrict(GridID visibleId) const
{
    validateItemsCache();

    return visibleId.row > m_idStart.row && visibleId.row < m_idEnd.row &&
            visibleId.column > m_idStart.column && visibleId.column < m_idEnd.column;
}

bool CacheSpaceGrid::isItemAbsInFrame(GridID absId) const
{
    auto visibleItem = spaceGrid()->toGridVisible(absId);
    if (!visibleItem.isValid())
        return false;

    return isItemInFrame(visibleItem);
}

void CacheSpaceGrid::visibleItemsRange(GridID& idStart, GridID& idEnd) const
{
    validateItemsCache();

    idStart = m_idStart;
    idEnd = m_idEnd;
}

GridID CacheSpaceGrid::visibleItemByPosition(QPoint point) const
{
    if (m_grid->isEmptyVisible())
        return GridID();

    QPoint gridPoint = window2Space(point);

    GridID visibleItem;
    visibleItem.row = m_grid->rows()->findVisibleIDByPos(gridPoint.y());
    visibleItem.column = m_grid->columns()->findVisibleIDByPos(gridPoint.x());

    return visibleItem;
}

void CacheSpaceGrid::clearItemsCacheImpl() const
{
    Q_ASSERT(!m_cacheIsInUse);

    m_idStart = m_idEnd = GridID();
    m_items.clear();
    m_scrollDelta = QPoint(0, 0);
    m_sizeDelta = QSize(0, 0);
}

void CacheSpaceGrid::validateItemsCacheImpl() const
{
    Q_ASSERT(m_itemsCacheInvalid);

    if (m_grid->isEmptyVisible())
    {
        clearItemsCache();
        m_itemsCacheInvalid = false;
        return;
    }

    auto_value<bool> inUse(m_cacheIsInUse, true);

    const Lines& rows = *m_grid->rows();
    const Lines& columns = *m_grid->columns();

    int visibleRowStart = rows.findVisibleIDByPos(m_scrollOffset.y());
    int visibleRowEnd = rows.findVisibleIDByPos(m_scrollOffset.y() + m_window.height());
    int visibleColumnStart = columns.findVisibleIDByPos(m_scrollOffset.x());
    int visibleColumnEnd = columns.findVisibleIDByPos(m_scrollOffset.x() + m_window.width());

    Q_ASSERT(visibleRowStart != InvalidIndex);
    Q_ASSERT(visibleRowEnd != InvalidIndex);
    Q_ASSERT(visibleColumnStart != InvalidIndex);
    Q_ASSERT(visibleColumnEnd != InvalidIndex);

    GridID newIdStart(visibleRowStart, visibleColumnStart);
    GridID newIdEnd(visibleRowEnd, visibleColumnEnd);

    if ((m_idStart == newIdStart) && (m_idEnd == newIdEnd))
    {
        // just offset rectangles
        for (const auto& item: m_items)
            item->correctRectangles(m_scrollDelta);

        // clear offset
        m_scrollDelta = QPoint(0, 0);
        m_sizeDelta = QSize(0, 0);
        // mark items as valid
        m_itemsCacheInvalid = false;
        return;
    }

    // init new items with empty caches
    int newIdRows = newIdEnd.row - newIdStart.row + 1;
    int newIdColumns = newIdEnd.column - newIdStart.column + 1;
    QVector<SharedPtr<CacheItem>> newItems(newIdRows * newIdColumns, nullptr);

    if (!m_items.isEmpty())
    {
        // find intersection between old and new items
        GridID intersectionStart(qMax(m_idStart.row, newIdStart.row), qMax(m_idStart.column, newIdStart.column));
        GridID intersectionEnd(qMin(m_idEnd.row, newIdEnd.row), qMin(m_idEnd.column, newIdEnd.column));

        // copy intersected cache items
        int oldIdColumns = m_idEnd.column - m_idStart.column + 1;
        for (GridID id = intersectionStart; id.column <= intersectionEnd.column; ++id.column)
            for (id.row = intersectionStart.row; id.row <= intersectionEnd.row; ++id.row)
            {
                GridID idNew = id - newIdStart;
                GridID idOld = id - m_idStart;
                auto& oldCacheItem = m_items[idOld.row * oldIdColumns + idOld.column];
                auto& newCacheItem = newItems[idNew.row * newIdColumns + idNew.column];
                newCacheItem.swap(oldCacheItem);
                newCacheItem->correctRectangles(m_scrollDelta);
            }
    }

    // initialize non-intersected cells
    QPoint origin = originPos();
    for (GridID idVisible = newIdStart; idVisible.column <= newIdEnd.column; ++idVisible.column)
    {
        for (idVisible.row = newIdStart.row; idVisible.row <= newIdEnd.row; ++idVisible.row)
        {
            GridID id = idVisible - newIdStart;

            auto& cacheItem = newItems[id.row * newIdColumns + id.column];
            if (cacheItem)
                continue;

            cacheItem = createCacheItem(ID(idVisible));
            // correct rectangle
            cacheItem->rect.translate(origin);
        }
    }

    m_idStart.swap(newIdStart);
    m_idEnd.swap(newIdEnd);
    m_items.swap(newItems);

    // clear offset
    m_scrollDelta = QPoint(0, 0);
    m_sizeDelta = QSize(0, 0);
    // mark items as valid
    m_itemsCacheInvalid = false;
}

bool CacheSpaceGrid::forEachCacheItemImpl(const std::function<bool (const SharedPtr<CacheItem> &)> &visitor) const
{
    for (const auto& cacheItem : m_items)
    {
        if (!visitor(cacheItem))
            return false;
    }
    return true;
}

const CacheItem* CacheSpaceGrid::cacheItemImpl(ID visibleId) const
{
    auto visId = visibleId.as<GridID>();

    if (!isItemInFrame(visId))
        return nullptr;

    int idColumns = m_idEnd.column - m_idStart.column + 1;
    auto id = visId - m_idStart;
    int index = id.row * idColumns + id.column;
    Q_ASSERT(index < m_items.size());
    return m_items[index].data();
}

const CacheItem* CacheSpaceGrid::cacheItemByPositionImpl(QPoint point) const
{
    validateItemsCache();

    if (isEmpty())
        return nullptr;

    QPoint gridPoint = window2Space(point);

    GridID visibleId;
    visibleId.row = m_grid->rows()->findVisibleIDByPos(gridPoint.y(), m_idStart.row, m_idEnd.row);
    visibleId.column = m_grid->columns()->findVisibleIDByPos(gridPoint.x(), m_idStart.column, m_idEnd.column);

    return cacheItem(ID(visibleId));
}

} // end namespace Qi
