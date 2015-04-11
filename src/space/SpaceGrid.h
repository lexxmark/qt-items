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

#ifndef QI_SPACE_GRID_H
#define QI_SPACE_GRID_H

#include "Space.h"
#include "Lines.h"
#include "core/ItemsIterator.h"
#include <QPoint>

namespace Qi
{

class ModelComparable;

enum SpaceGridHint
{
    SpaceGridHintNone = 0x0000,
    SpaceGridHintSameSchemasByColumn = 0x0001,
    SpaceGridHintSameSchemasByRow = 0x0002,
};

class QI_EXPORT SpaceGrid: public Space
{
    Q_OBJECT
    Q_DISABLE_COPY(SpaceGrid)

public:
    SpaceGrid(SpaceGridHint hint = SpaceGridHintNone);
    SpaceGrid(const QSharedPointer<Lines>& rows, const QSharedPointer<Lines>& columns, SpaceGridHint hint = SpaceGridHintNone);
    ~SpaceGrid();

    SpaceGridHint hint() const { return m_hint; }
    void setHint(SpaceGridHint hint);

    QSize size() const override;
    ItemID toAbsolute(const ItemID& visibleItem) const override { return ItemID(m_rows->toAbsoluteSafe(visibleItem.row), m_columns->toAbsoluteSafe(visibleItem.column)); }
    ItemID toVisible(const ItemID& absoluteItem) const override { return ItemID(m_rows->toVisibleSafe(absoluteItem.row), m_columns->toVisibleSafe(absoluteItem.column)); }
    QRect itemRect(const ItemID& visibleItem) const override;
    QSharedPointer<CacheItemFactory> createCacheItemFactory(ViewApplicationMask viewApplicationMask = ViewApplicationNone) const override;

    bool isEmpty() const { return m_rows->isEmpty() || m_columns->isEmpty(); }
    bool isEmptyVisible() const { return m_rows->isEmptyVisible() || m_columns->isEmptyVisible(); }

    const QSharedPointer<Lines>& rows() const { return m_rows; }
    const QSharedPointer<Lines>& columns() const { return m_columns; }

    void setDimensions(int rows, int columns);
    void setRowsCount(int rows) { m_rows->setCount(rows); }
    void setColumnsCount(int columns) { m_columns->setCount(columns); }

    QSize dimensions() const;
    int rowsCount() const { return m_rows->count(); }
    int columnsCount() const { return m_columns->count(); }

    int rowsVisibleCount() const { return m_rows->visibleCount(); }
    int columnsVisibleCount() const { return m_columns->visibleCount(); }

    void shareRows(const QSharedPointer<Lines>& rows);
    void shareColumns(const QSharedPointer<Lines>& columns);

    void unshareRows();
    void unshareColumns();

    QSize itemSize(const ItemID& item) const;

    bool checkItem(const ItemID& item) const;
    bool checkVisibleItem(const ItemID& item) const;
    bool isItemVisible(const ItemID& item) const;

    void sortColumnByModel(int column, const QSharedPointer<ModelComparable>& model, bool ascending, bool stable);
    void sortRowByModel(int row, const QSharedPointer<ModelComparable>& model, bool ascending, bool stable);

    void sortColumnByRangedModel(int column, const QSharedPointer<ModelComparable>& model, const QSharedPointer<Range>& range, bool ascending, bool stable, bool outOfRangeIsSmall);
    void sortRowByRangedModel(int row, const QSharedPointer<ModelComparable>& model, const QSharedPointer<Range>& range, bool ascending, bool stable, bool outOfRangeIsSmall);

private slots:
    void onLinesChanged(const Lines* lines, ChangeReason reason);

private:
    void connectLines(const QSharedPointer<Lines>& lines);
    void disconnectLines(const QSharedPointer<Lines>& lines);

    ItemID trimItem(const ItemID& item) const;
    ItemID trimVisibleItem(const ItemID& item) const;

    QSharedPointer<Lines> m_rows;
    QSharedPointer<Lines> m_columns;

    SpaceGridHint m_hint;
};

QI_EXPORT QSharedPointer<Range> createItemRangeRect(const SpaceGrid& grid, const ItemID& displayCorner1, const ItemID& displayCorner2);

class QI_EXPORT ItemsIteratorGrid: public ItemsIterator
{
public:
    explicit ItemsIteratorGrid(const SpaceGrid& spaceGrid);

protected:
    ItemID itemImpl() const override { return m_currentItem; }
    bool atFirstImpl() override;
    bool toNextImpl() override;

private:
    const SpaceGrid& m_spaceGrid;
    ItemID m_currentItem;
};

class QI_EXPORT ItemsIteratorGridVisible: public ItemsIterator
{
public:
    explicit ItemsIteratorGridVisible(const SpaceGrid& spaceGrid);

    ItemID itemVisible() const { return m_currentItemVisible; }

protected:
    ItemID itemImpl() const override { return m_currentItem; }
    bool atFirstImpl() override;
    bool toNextImpl() override;

private:
    const SpaceGrid& m_spaceGrid;
    ItemID m_currentItemVisible;
    ItemID m_currentItem;
};

class QI_EXPORT ItemsIteratorGridByColumn: public ItemsIterator
{
public:
    explicit ItemsIteratorGridByColumn(const SpaceGrid& spaceGrid, int column = 0);

protected:
    ItemID itemImpl() const override { return m_currentItem; }
    bool atFirstImpl() override;
    bool toNextImpl() override;

private:
    const Lines& m_rows;
    ItemID m_currentItem;
};


} // end namespace Qi 

#endif // QI_SPACE_GRID_H
