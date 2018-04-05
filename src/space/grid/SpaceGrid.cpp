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

#include "SpaceGrid.h"
#include "RangeGrid.h"
#include "core/Model.h"
#include "cache/CacheItemFactory.h"

namespace Qi
{

SharedPtr<Range> makeRangeGridRect(const SpaceGrid& grid, GridID displayCorner1, GridID displayCorner2)
{
    if (grid.isEmptyVisible())
        return nullptr;

    if (!grid.checkVisibleItem(displayCorner1) || !grid.checkVisibleItem(displayCorner2))
        return nullptr;

    GridID itemTopLeft(qMin(displayCorner1.row, displayCorner2.row), qMin(displayCorner1.column, displayCorner2.column));
    GridID itemBottomRight(qMax(displayCorner1.row, displayCorner2.row), qMax(displayCorner1.column, displayCorner2.column));

    QSet<int> rows;
    for (int row = itemTopLeft.row; row <= itemBottomRight.row; ++row)
    {
        rows.insert(grid.rows()->toAbsolute(row));
    }

    QSet<int> columns;
    for (int column = itemTopLeft.column; column <= itemBottomRight.column; ++column)
    {
        columns.insert(grid.columns()->toAbsolute(column));
    }

    return makeRangeGridRect(rows, columns);
}

IdIteratorGridAll::IdIteratorGridAll(const SpaceGrid& spaceGrid)
    : m_spaceGrid(spaceGrid)
{
    atFirst();
}

bool IdIteratorGridAll::atFirstImpl()
{
    if (m_spaceGrid.isEmpty())
    {
        m_currentItem = GridID();
        return false;
    }

    m_currentItem = GridID(0, 0);
    return true;
}

bool IdIteratorGridAll::toNextImpl()
{
    if (!m_currentItem.isValid())
        return false;

    ++m_currentItem.column;

    for (;m_currentItem.row < m_spaceGrid.rowsCount(); ++m_currentItem.row, m_currentItem.column = 0)
    {
        if (m_currentItem.column < m_spaceGrid.columnsCount())
            return true;
    }

    m_currentItem = GridID();
    return false;
}


ItemsIteratorGridVisible::ItemsIteratorGridVisible(const SpaceGrid& spaceGrid)
    : m_spaceGrid(spaceGrid)
{
    atFirst();
}

bool ItemsIteratorGridVisible::atFirstImpl()
{
    if (m_spaceGrid.isEmptyVisible())
    {
        m_currentItem = GridID();
        m_currentItemVisible = GridID();
        return false;
    }

    m_currentItemVisible = GridID(0, 0);
    m_currentItem = m_spaceGrid.toGridAbsolute(m_currentItemVisible);
    return true;
}

bool ItemsIteratorGridVisible::toNextImpl()
{
    if (!m_currentItemVisible.isValid())
        return false;

    ++m_currentItemVisible.column;

    for (;m_currentItemVisible.row < m_spaceGrid.rowsVisibleCount(); ++m_currentItemVisible.row, m_currentItemVisible.column = 0)
    {
        if (m_currentItemVisible.column < m_spaceGrid.columnsVisibleCount())
        {
            m_currentItem = m_spaceGrid.toGridAbsolute(m_currentItemVisible);
            return true;
        }
    }

    m_currentItemVisible = GridID();
    m_currentItem = GridID();
    return false;
}

ItemsIteratorGridByColumn::ItemsIteratorGridByColumn(const SpaceGrid& spaceGrid, int column)
    : m_rows(*spaceGrid.rows()),
      m_currentItem(InvalidIndex, column)
{
    Q_ASSERT(column >= 0 && column < spaceGrid.columnsCount());
    atFirst();
}

bool ItemsIteratorGridByColumn::atFirstImpl()
{
    if (m_rows.isEmpty())
    {
        m_currentItem.row = InvalidIndex;
        return false;
    }

    m_currentItem.row = 0;
    return true;
}

bool ItemsIteratorGridByColumn::toNextImpl()
{
    if (!m_currentItem.isValid())
        return false;

    ++m_currentItem.row;

    if (m_currentItem.row < m_rows.count())
        return true;

    m_currentItem.row = InvalidIndex;
    return false;
}

class AscendingColumnComparatorByModel
{
public:
    AscendingColumnComparatorByModel(int column, const ModelComparable& model)
        : m_leftItem(0, column),
          m_rightItem(0, column),
          m_model(model)
    {}

    bool operator() (int left, int right)
    {
        m_leftItem.row = left;
        m_rightItem.row = right;
        return m_model.compareAs(m_leftItem, m_rightItem) < 0;
    }

private:
    GridID m_leftItem;
    GridID m_rightItem;
    const ModelComparable& m_model;
};

class DescendingColumnComparatorByModel
{
public:
    DescendingColumnComparatorByModel(int column, const ModelComparable& model)
        : m_leftItem(0, column),
        m_rightItem(0, column),
        m_model(model)
    {}

    bool operator() (int left, int right)
    {
        m_leftItem.row = left;
        m_rightItem.row = right;
        return m_model.compareAs(m_leftItem, m_rightItem) > 0;
    }

private:
    GridID m_leftItem;
    GridID m_rightItem;
    const ModelComparable& m_model;
};

class AscendingRowComparatorByModel
{
public:
    AscendingRowComparatorByModel(int row, const ModelComparable& model)
        : m_leftItem(row, 0),
        m_rightItem(row, 0),
        m_model(model)
    {}

    bool operator() (int left, int right)
    {
        m_leftItem.column = left;
        m_rightItem.column = right;
        return m_model.compareAs(m_leftItem, m_rightItem) < 0;
    }

private:
    GridID m_leftItem;
    GridID m_rightItem;
    const ModelComparable& m_model;
};

class DescendingRowComparatorByModel
{
public:
    DescendingRowComparatorByModel(int row, const ModelComparable& model)
        : m_leftItem(row, 0),
        m_rightItem(row, 0),
        m_model(model)
    {}

    bool operator() (int left, int right)
    {
        m_leftItem.column = left;
        m_rightItem.column = right;
        return m_model.compareAs(m_leftItem, m_rightItem) > 0;
    }

private:
    GridID m_leftItem;
    GridID m_rightItem;
    const ModelComparable& m_model;
};

SpaceGrid::SpaceGrid(SpaceGridHint hint)
    : m_rows(new Lines()),
      m_columns(new Lines()),
      m_hint(hint)
{
    connectLines(m_rows);
    connectLines(m_columns);
}

SpaceGrid::SpaceGrid(SharedPtr<Lines> rows, SharedPtr<Lines> columns, SpaceGridHint hint)
    : m_rows(rows),
      m_columns(columns),
      m_hint(hint)
{
    connectLines(m_rows);
    connectLines(m_columns);
}

SpaceGrid::~SpaceGrid()
{
    disconnectLines(m_rows);
    disconnectLines(m_columns);
}

void SpaceGrid::setHint(SpaceGridHint hint)
{
    if (m_hint == hint)
        return;

    m_hint = hint;
    emit spaceChanged(this, ChangeReasonSpaceHint);
}

void SpaceGrid::setDimensions(int rows, int columns)
{
    m_rows->setCount(rows);
    m_columns->setCount(columns);
}

QSize SpaceGrid::dimensions() const
{
    return QSize((int)m_columns->count(), (int)m_rows->count());
}

QSize SpaceGrid::size() const
{
    return QSize((int)m_columns->visibleSize(), (int)m_rows->visibleSize());
}

GridID SpaceGrid::toGridAbsolute(GridID visibleItem) const
{
    return GridID(m_rows->toAbsoluteSafe(visibleItem.row), m_columns->toAbsoluteSafe(visibleItem.column));
}

GridID SpaceGrid::toGridVisible(GridID absoluteItem) const
{
    return GridID(m_rows->toVisibleSafe(absoluteItem.row), m_columns->toVisibleSafe(absoluteItem.column));
}

QRect SpaceGrid::itemRect(ID visibleItem) const
{
    const auto& id = visibleItem.as<GridID>();
    Q_ASSERT(id.isValid());
    Q_ASSERT(checkVisibleItem(id));

    QRect rect(0, 0, 0, 0);
    rect.setTop(m_rows->startPos(id.row));
    rect.setLeft(m_columns->startPos(id.column));
    rect.setBottom(m_rows->endPos(id.row));
    rect.setRight(m_columns->endPos(id.column));

    return rect;
}

SharedPtr<CacheItemFactory> SpaceGrid::createCacheItemFactory(ViewApplicationMask viewApplicationMask) const
{
    switch (m_hint) {
    case SpaceGridHintSameSchemasByColumn:
        return createCacheItemFactorySameSchemaByColumn(*this, viewApplicationMask);
    case SpaceGridHintSameSchemasByRow:
        return createCacheItemFactorySameSchemaByRow(*this, viewApplicationMask);
    default:
        return createCacheItemFactoryDefault(*this, viewApplicationMask);
    }
}

QSize SpaceGrid::itemSize(GridID item) const
{
    return QSize((int)m_columns->lineSize(item.column), (int)m_rows->lineSize(item.row));
}

void SpaceGrid::connectLines(const SharedPtr<Lines> &lines)
{
    connect(lines.data(), &Lines::linesChanged, this, &SpaceGrid::onLinesChanged);
}

void SpaceGrid::disconnectLines(const SharedPtr<Lines> &lines)
{
    disconnect(lines.data(), &Lines::linesChanged, this, &SpaceGrid::onLinesChanged);
}

GridID SpaceGrid::trimItem(GridID item) const
{
    Q_ASSERT(item.isValid());
    return GridID(qMin(item.row, m_rows->count() - 1), qMin(item.column, m_columns->count() - 1));
}

GridID SpaceGrid::trimVisibleItem(GridID item) const
{
    Q_ASSERT(item.isValid());
    return GridID(qMin(item.row, m_rows->visibleCount() - 1), qMin(item.column, m_columns->visibleCount() - 1));
}

void SpaceGrid::unshareRows()
{
    disconnectLines(m_rows);
    m_rows = m_rows->clone();
    connectLines(m_rows);

    emit spaceChanged(this, ChangeReasonLinesCount);
}

void SpaceGrid::unshareColumns()
{
    disconnectLines(m_columns);
    m_columns = m_columns->clone();
    connectLines(m_columns);

    emit spaceChanged(this, ChangeReasonLinesCount);
}

void SpaceGrid::shareRows(SharedPtr<Lines> rows)
{
    disconnectLines(m_rows);
    m_rows = std::move(rows);
    connectLines(m_rows);

    emit spaceChanged(this, ChangeReasonLinesCount);
}

void SpaceGrid::shareColumns(SharedPtr<Lines> columns)
{
    disconnectLines(m_columns);
    m_columns = std::move(columns);
    connectLines(m_columns);

    emit spaceChanged(this, ChangeReasonLinesCount);
}

bool SpaceGrid::checkItem(GridID item) const
{
    return item.row < m_rows->count() && item.column < m_columns->count();
}

bool SpaceGrid::checkVisibleItem(GridID item) const
{
    return item.row < m_rows->visibleCount() && item.column < m_columns->visibleCount();
}

bool SpaceGrid::isItemVisible(GridID item) const
{
    if (!checkItem(item))
        return false;

    return m_rows->isLineVisible(item.row) && m_columns->isLineVisible(item.column);
}

void SpaceGrid::sortColumnByModel(int column, const ModelComparable& model, bool ascending, bool stable)
{
    // avoid invalid column
    if (column >= m_columns->count())
        return;

    if (ascending)
        m_rows->sort(stable, AscendingColumnComparatorByModel(column, model));
    else
        m_rows->sort(stable, DescendingColumnComparatorByModel(column, model));
}

void SpaceGrid::sortRowByModel(int row, const ModelComparable &model, bool ascending, bool stable)
{
    // avoid invalid row
    if (row >= m_rows->count())
        return;

    if (ascending)
        m_columns->sort(stable, AscendingRowComparatorByModel(row, model));
    else
        m_columns->sort(stable, DescendingRowComparatorByModel(row, model));
}

void SpaceGrid::onLinesChanged(const Lines* /*lines*/, ChangeReason reason)
{
    if (reason & (ChangeReasonLinesCount|ChangeReasonLinesVisibility|ChangeReasonLinesSize|ChangeReasonLinesOrder))
    {
        emit spaceChanged(this, ChangeReasonSpaceStructure);
    }
}

} // end namespace Qi
