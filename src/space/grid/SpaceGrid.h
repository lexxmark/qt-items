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

#include "space/Space.h"
#include "GridID.h"
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
    SpaceGrid(SharedPtr<Lines> rows, SharedPtr<Lines> columns, SpaceGridHint hint = SpaceGridHintNone);
    ~SpaceGrid();

    SpaceGridHint hint() const { return m_hint; }
    void setHint(SpaceGridHint hint);

    QSize size() const override;
    ID toAbsolute(ID visibleItem) const override { return ID(toGridAbsolute(visibleItem.as<GridID>())); }
    ID toVisible(ID absoluteItem) const override { return ID(toGridVisible(absoluteItem.as<GridID>())); }
    QRect itemRect(ID visibleItem) const override;
    SharedPtr<CacheItemFactory> createCacheItemFactory() const override;

    bool isEmpty() const { return m_rows->isEmpty() || m_columns->isEmpty(); }
    bool isEmptyVisible() const { return m_rows->isEmptyVisible() || m_columns->isEmptyVisible(); }

    const SharedPtr<Lines>& rows() const { return m_rows; }
    const SharedPtr<Lines>& columns() const { return m_columns; }

    void setDimensions(int rows, int columns);
    void setRowsCount(int rows) { m_rows->setCount(rows); }
    void setColumnsCount(int columns) { m_columns->setCount(columns); }

    QSize dimensions() const;
    int rowsCount() const { return m_rows->count(); }
    int columnsCount() const { return m_columns->count(); }

    int rowsVisibleCount() const { return m_rows->visibleCount(); }
    int columnsVisibleCount() const { return m_columns->visibleCount(); }

    void shareRows(SharedPtr<Lines> rows);
    void shareColumns(SharedPtr<Lines> columns);

    void unshareRows();
    void unshareColumns();

    GridID toGridAbsolute(GridID visibleItem) const;
    GridID toGridVisible(GridID absoluteItem) const;

    QSize itemSize(GridID id) const;

    bool checkItem(GridID id) const;
    bool checkVisibleItem(GridID id) const;
    bool isItemVisible(GridID id) const;

    void sortColumnByModel(int column, const ModelComparable &model, bool ascending, bool stable);
    void sortRowByModel(int row, const ModelComparable& model, bool ascending, bool stable);

private slots:
    void onLinesChanged(const Lines* lines, ChangeReason reason);

private:
    void connectLines(const SharedPtr<Lines>& lines);
    void disconnectLines(const SharedPtr<Lines>& lines);

    GridID trimItem(GridID item) const;
    GridID trimVisibleItem(GridID item) const;

    SharedPtr<Lines> m_rows;
    SharedPtr<Lines> m_columns;

    SpaceGridHint m_hint;
};

QI_EXPORT SharedPtr<Range> makeRangeGridRect(const SpaceGrid& grid, GridID displayCorner1, GridID displayCorner2);

class QI_EXPORT IdIteratorGrid: public IdIterator
{
public:
    GridID gridId() const { return gridIdImpl(); }

protected:
    IdIteratorGrid() = default;

    ID idImpl() const final { return ID(gridId()); }
    bool isValidImpl() const final { return gridId().isValid(); }

    virtual GridID gridIdImpl() const = 0;
};

class QI_EXPORT IdIteratorGridAll: public IdIteratorGrid
{
public:
    explicit IdIteratorGridAll(const SpaceGrid& spaceGrid);

protected:
    GridID gridIdImpl() const override { return m_currentItem; }
    bool atFirstImpl() override;
    bool toNextImpl() override;

private:
    const SpaceGrid& m_spaceGrid;
    GridID m_currentItem;
};

class QI_EXPORT ItemsIteratorGridVisible: public IdIteratorGrid
{
public:
    explicit ItemsIteratorGridVisible(const SpaceGrid& spaceGrid);

    GridID gridIdVisible() const { return m_currentItemVisible; }

protected:
    GridID gridIdImpl() const override { return m_currentItem; }
    bool atFirstImpl() override;
    bool toNextImpl() override;

private:
    const SpaceGrid& m_spaceGrid;
    GridID m_currentItemVisible;
    GridID m_currentItem;
};

class QI_EXPORT ItemsIteratorGridByColumn: public IdIteratorGrid
{
public:
    explicit ItemsIteratorGridByColumn(const SpaceGrid& spaceGrid, int column = 0);

protected:
    GridID gridIdImpl() const override { return m_currentItem; }
    bool atFirstImpl() override;
    bool toNextImpl() override;

private:
    const Lines& m_rows;
    GridID m_currentItem;
};


} // end namespace Qi 

#endif // QI_SPACE_GRID_H
