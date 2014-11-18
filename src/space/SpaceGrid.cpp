#include "SpaceGrid.h"
#include "core/Model.h"
#include "core/ext/Ranges.h"

namespace Qi
{

QSharedPointer<Range> CreateCellRangeRect(const SpaceGrid& grid, const ItemID& displayCorner1, const ItemID& displayCorner2)
{
    if (grid.isEmptyVisible())
        return QSharedPointer<Range>();

    if (!grid.checkVisibleItem(displayCorner1) || !grid.checkVisibleItem(displayCorner2))
        return QSharedPointer<Range>();

    ItemID itemTopLeft(std::min(displayCorner1.row, displayCorner2.row), std::min(displayCorner1.column, displayCorner2.column));
    ItemID itemBottomRight(std::max(displayCorner1.row, displayCorner2.row), std::max(displayCorner1.column, displayCorner2.column));

    QSet<quint32> rows;
    for (quint32 row = itemTopLeft.row; row <= itemBottomRight.row; ++row)
    {
        rows.insert(grid.rows()->toAbsolute(row));
    }

    QSet<quint32> columns;
    for (quint32 column = itemTopLeft.column; column <= itemBottomRight.column; ++column)
    {
        columns.insert(grid.columns()->toAbsolute(column));
    }

    return makeRangeRect(rows, columns);
}

class AscendingColumnComparatorByModel
{
public:
    AscendingColumnComparatorByModel(quint32 column, const QSharedPointer<ModelComparable>& model)
        : m_leftItem(0, column),
          m_rightItem(0, column),
          m_model(model)
    {}

    bool operator() (quint32 left, quint32 right)
    {
        m_leftItem.row = left;
        m_rightItem.row = right;
        return m_model->compare(m_leftItem, m_rightItem) < 0;
    }

private:
    ItemID m_leftItem;
    ItemID m_rightItem;
    QSharedPointer<ModelComparable> m_model;
};

class DescendingColumnComparatorByModel
{
public:
    DescendingColumnComparatorByModel(quint32 column, const QSharedPointer<ModelComparable>& model)
        : m_leftItem(0, column),
        m_rightItem(0, column),
        m_model(model)
    {}

    bool operator() (quint32 left, quint32 right)
    {
        m_leftItem.row = left;
        m_rightItem.row = right;
        return m_model->compare(m_leftItem, m_rightItem) > 0;
    }

private:
    ItemID m_leftItem;
    ItemID m_rightItem;
    QSharedPointer<ModelComparable> m_model;
};

class AscendingRowComparatorByModel
{
public:
    AscendingRowComparatorByModel(quint32 row, const QSharedPointer<ModelComparable>& model)
        : m_leftItem(row, 0),
        m_rightItem(row, 0),
        m_model(model)
    {}

    bool operator() (quint32 left, quint32 right)
    {
        m_leftItem.column = left;
        m_rightItem.column = right;
        return m_model->compare(m_leftItem, m_rightItem) < 0;
    }

private:
    ItemID m_leftItem;
    ItemID m_rightItem;
    QSharedPointer<ModelComparable> m_model;
};

class DescendingRowComparatorByModel
{
public:
    DescendingRowComparatorByModel(quint32 row, const QSharedPointer<ModelComparable>& model)
        : m_leftItem(row, 0),
        m_rightItem(row, 0),
        m_model(model)
    {}

    bool operator() (quint32 left, quint32 right)
    {
        m_leftItem.column = left;
        m_rightItem.column = right;
        return m_model->compare(m_leftItem, m_rightItem) > 0;
    }

private:
    ItemID m_leftItem;
    ItemID m_rightItem;
    QSharedPointer<ModelComparable> m_model;
};

struct ModelRangeInfo
{
protected:
    ModelRangeInfo(const QSharedPointer<ModelComparable>& model, const QSharedPointer<Range>& range, bool outOfRangeIsSmall)
        : m_model(model),
        m_range(range),
        m_outOfRangeIsSmall(outOfRangeIsSmall)
    {}

    QSharedPointer<ModelComparable> m_model;
    QSharedPointer<Range> m_range;
    bool m_outOfRangeIsSmall;
};

class AscendingColumnComparatorByRangedModel : public ModelRangeInfo
{
public:
    AscendingColumnComparatorByRangedModel(quint32 column, const QSharedPointer<ModelComparable>& model, const QSharedPointer<Range>& range, bool outOfRangeIsSmall)
        : ModelRangeInfo(model, range, outOfRangeIsSmall),
        m_leftItem(0, column),
        m_rightItem(0, column)
    {}

    bool operator() (quint32 left, quint32 right)
    {
        m_leftItem.row = left;
        m_rightItem.row = right;

        bool leftIsOut = !m_range->hasItem(m_leftItem);
        bool rightIsOut = !m_range->hasItem(m_rightItem);

        if (leftIsOut && rightIsOut)
            return false;
        else if (leftIsOut)
            return m_outOfRangeIsSmall;
        else if (rightIsOut)
            return !m_outOfRangeIsSmall;

        return m_model->compare(m_leftItem, m_rightItem) < 0;
    }

private:
    ItemID m_leftItem;
    ItemID m_rightItem;
};

class DescendingColumnComparatorByRangedModel : public ModelRangeInfo
{
public:
    DescendingColumnComparatorByRangedModel(quint32 column, const QSharedPointer<ModelComparable>& model, const QSharedPointer<Range>& range, bool outOfRangeIsSmall)
        : ModelRangeInfo(model, range, outOfRangeIsSmall),
        m_leftItem(0, column),
        m_rightItem(0, column)
    {}

    bool operator() (quint32 left, quint32 right)
    {
        m_leftItem.row = left;
        m_rightItem.row = right;

        bool leftIsOut = !m_range->hasItem(m_leftItem);
        bool rightIsOut = !m_range->hasItem(m_rightItem);

        if (leftIsOut && rightIsOut)
            return false;
        else if (leftIsOut)
            return m_outOfRangeIsSmall;
        else if (rightIsOut)
            return !m_outOfRangeIsSmall;

        return m_model->compare(m_leftItem, m_rightItem) > 0;
    }

private:
    ItemID m_leftItem;
    ItemID m_rightItem;
};

class AscendingRowComparatorByRangedModel : public ModelRangeInfo
{
public:
    AscendingRowComparatorByRangedModel(quint32 row, const QSharedPointer<ModelComparable>& model, const QSharedPointer<Range>& range, bool outOfRangeIsSmall)
        : ModelRangeInfo(model, range, outOfRangeIsSmall),
        m_leftItem(row, 0),
        m_rightItem(row, 0)
    {}

    bool operator() (quint32 left, quint32 right)
    {
        m_leftItem.column = left;
        m_rightItem.column = right;

        bool leftIsOut = !m_range->hasItem(m_leftItem);
        bool rightIsOut = !m_range->hasItem(m_rightItem);

        if (leftIsOut && rightIsOut)
            return false;
        else if (leftIsOut)
            return m_outOfRangeIsSmall;
        else if (rightIsOut)
            return !m_outOfRangeIsSmall;

        return m_model->compare(m_leftItem, m_rightItem) < 0;
    }

private:
    ItemID m_leftItem;
    ItemID m_rightItem;
};

class DescendingRowComparatorByRangedModel : public ModelRangeInfo
{
public:
    DescendingRowComparatorByRangedModel(quint32 row, const QSharedPointer<ModelComparable>& model, const QSharedPointer<Range>& range, bool outOfRangeIsSmall)
        : ModelRangeInfo(model, range, outOfRangeIsSmall),
        m_leftItem(row, 0),
        m_rightItem(row, 0)
    {}

    bool operator() (quint32 left, quint32 right)
    {
        m_leftItem.column = left;
        m_rightItem.column = right;

        bool leftIsOut = !m_range->hasItem(m_leftItem);
        bool rightIsOut = !m_range->hasItem(m_rightItem);

        if (leftIsOut && rightIsOut)
            return false;
        else if (leftIsOut)
            return m_outOfRangeIsSmall;
        else if (rightIsOut)
            return !m_outOfRangeIsSmall;

        return m_model->compare(m_leftItem, m_rightItem) > 0;
    }

private:
    ItemID m_leftItem;
    ItemID m_rightItem;
};

SpaceGrid::SpaceGrid(SpaceHints hints)
    : Space(hints),
      m_rows(new Lines()),
      m_columns(new Lines())
{
    connectLines(m_rows);
    connectLines(m_columns);
}

SpaceGrid::SpaceGrid(const QSharedPointer<Lines>& rows, const QSharedPointer<Lines>& columns, SpaceHints hints)
    : Space(hints),
      m_rows(rows),
      m_columns(columns)
{
    connectLines(m_rows);
    connectLines(m_columns);
}

SpaceGrid::~SpaceGrid()
{
    disconnectLines(m_rows);
    disconnectLines(m_columns);
}

void SpaceGrid::setDimensions(quint32 rows, quint32 columns)
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

QRect SpaceGrid::itemRect(const ItemID& visibleItem) const
{
    QRect rect(0, 0, 0, 0);
    if (visibleItem.row > 0)
        rect.setTop(m_rows->sizeAtVisLine(visibleItem.row - 1));
    if (visibleItem.column > 0)
        rect.setLeft(m_columns->sizeAtVisLine(visibleItem.column - 1));

    rect.setBottom(m_rows->sizeAtVisLine(visibleItem.row));
    rect.setRight(m_columns->sizeAtVisLine(visibleItem.column));

    return rect;
}

QSize SpaceGrid::itemSize(const ItemID& item) const
{
    return QSize((int)m_columns->lineSize(item.column), (int)m_rows->lineSize(item.row));
}

void SpaceGrid::connectLines(const QSharedPointer<Lines>& lines)
{
    connect(lines.data(), &Lines::linesChanged, this, &SpaceGrid::onLinesChanged);
}

void SpaceGrid::disconnectLines(const QSharedPointer<Lines>& lines)
{
    disconnect(lines.data(), &Lines::linesChanged, this, &SpaceGrid::onLinesChanged);
}

ItemID SpaceGrid::trimItem(const ItemID& item) const
{
    Q_ASSERT(item.isValid());
    return ItemID(std::min(item.row, m_rows->count() - 1), std::min(item.column, m_columns->count() - 1));
}

ItemID SpaceGrid::trimVisibleItem(const ItemID& item) const
{
    Q_ASSERT(item.isValid());
    return ItemID(std::min(item.row, m_rows->visibleCount() - 1), std::min(item.column, m_columns->visibleCount() - 1));
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

void SpaceGrid::shareRows(const QSharedPointer<Lines>& rows)
{
    disconnectLines(m_rows);
    m_rows = rows;
    connectLines(m_rows);

    emit spaceChanged(this, ChangeReasonLinesCount);
}

void SpaceGrid::shareColumns(const QSharedPointer<Lines>& columns)
{
    disconnectLines(m_columns);
    m_columns = columns;
    connectLines(m_columns);

    emit spaceChanged(this, ChangeReasonLinesCount);
}

bool SpaceGrid::checkItem(const ItemID& item) const
{
    return item.row < m_rows->count() && item.column < m_columns->count();
}

bool SpaceGrid::checkVisibleItem(const ItemID& item) const
{
    return item.row < m_rows->visibleCount() && item.column < m_columns->visibleCount();
}

bool SpaceGrid::isItemVisible(const ItemID& item) const
{
    if (!checkItem(item))
        return false;

    return m_rows->isLineVisible(item.row) && m_columns->isLineVisible(item.column);
}

void SpaceGrid::sortColumnByModel(quint32 column, const QSharedPointer<ModelComparable>& model, bool ascending, bool stable)
{
    // avoid invalid column
    if (column >= m_columns->count())
        return;

    if (ascending)
        m_rows->sort(stable, AscendingColumnComparatorByModel(column, model));
    else
        m_rows->sort(stable, DescendingColumnComparatorByModel(column, model));
}

void SpaceGrid::sortRowByModel(quint32 row, const QSharedPointer<ModelComparable>& model, bool ascending, bool stable)
{
    // avoid invalid row
    if (row >= m_rows->count())
        return;

    if (ascending)
        m_columns->sort(stable, AscendingRowComparatorByModel(row, model));
    else
        m_columns->sort(stable, DescendingRowComparatorByModel(row, model));
}

void SpaceGrid::sortColumnByRangedModel(quint32 column, const QSharedPointer<ModelComparable>& model, const QSharedPointer<Range>& range, bool ascending, bool stable, bool outOfRangeIsSmall)
{
    // avoid invalid column
    if (column >= m_columns->count())
        return;

    if (ascending)
        m_rows->sort(stable, AscendingColumnComparatorByRangedModel(column, model, range, outOfRangeIsSmall));
    else
        m_rows->sort(stable, DescendingColumnComparatorByRangedModel(column, model, range, outOfRangeIsSmall));
}

void SpaceGrid::sortRowByRangedModel(quint32 row, const QSharedPointer<ModelComparable>& model, const QSharedPointer<Range>& range, bool ascending, bool stable, bool outOfRangeIsSmall)
{
    // avoid invalid row
    if (row >= m_rows->count())
        return;

    if (ascending)
        m_columns->sort(stable, AscendingRowComparatorByRangedModel(row, model, range, outOfRangeIsSmall));
    else
        m_columns->sort(stable, DescendingRowComparatorByRangedModel(row, model, range, outOfRangeIsSmall));
}

void SpaceGrid::onLinesChanged(const Lines* lines, ChangeReason reason)
{
    if (reason & (ChangeReasonLinesCount|ChangeReasonLineVisibility|ChangeReasonLineSize|ChangeReasonLineOrder))
    {
        emit spaceChanged(this, ChangeReasonSpaceStructure);
    }
}

} // end namespace Qi
