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

#ifndef QI_MODEL_STORE_H
#define QI_MODEL_STORE_H

#include "ModelTyped.h"
#include "space/SpaceGrid.h"
#include <QSet>
#include <functional>

namespace Qi
{

template <typename T, typename StorageT = typename std::decay<T>::type>
class ModelStorageGrid: public ModelTyped<T>
{
public:
    ModelStorageGrid(const QSharedPointer<SpaceGrid>& grid)
        : m_grid(grid),
          m_rowsCount(0)
    {
        Q_ASSERT(grid);
        m_connection = QObject::connect(grid.data(), &Space::spaceChanged, [this] (const Space* space, ChangeReason reason) {
            onSpaceChanged(space, reason);
        });
        resize();
    }

    ~ModelStorageGrid()
    {
        QObject::disconnect(m_connection);
    }

protected:
    T valueImpl(const ItemID& item) const override
    {
        int index = item.column * m_rowsCount + item.row;
        if (index >= m_values.size())
            throw std::logic_error("Cannot return value");

        return m_values[index];
    }

    bool setValueImpl(const ItemID& item, T value) override
    {
        int index = item.column * m_rowsCount + item.row;
        if (index < m_values.size())
        {
            m_values[index] = value;
            return true;
        }
        else
        {
            return false;
        }
    }

private slots:
    void onSpaceChanged(const Space* space, ChangeReason reason)
    {
        Q_UNUSED(space);
        if ((reason & ChangeReasonSpaceStructure) &&
            (reason & ChangeReasonLinesCount))
        {
            Q_ASSERT(space == m_grid.data());
            resize();
        }
    }

private:
    void resize()
    {
        auto grid = m_grid.toStrongRef();
        m_rowsCount = grid->rowsCount();
        m_values.resize(grid->rowsCount()*grid->columnsCount());
    }

    QWeakPointer<SpaceGrid> m_grid;
    QVector<StorageT> m_values;
    int m_rowsCount;
    QMetaObject::Connection m_connection;
};

template <typename T, typename StorageT = typename std::decay<T>::type, typename NotEq = typename std::not_equal_to<T>>
class ModelStorageValue: public ModelTyped<T>
{
public:
    ModelStorageValue(T value)
        : m_value(value)
    {
    }

protected:
    T valueImpl(const ItemID& /*item*/) const override
    {
        return m_value;
    }

    bool setValueImpl(const ItemID& /*item*/, T value) override
    {
        if (NotEq()(m_value, value))
        {
            m_value = value;
            return true;
        }

        return false;
    }

private:
    StorageT m_value;
};

template <typename T, typename StorageT = typename std::decay<T>::type>
class ModelStorageColumns: public ModelTyped<T>
{
public:
    ModelStorageColumns(const QSharedPointer<Lines>& rows, const QSet<int>& columns) { init(rows, columns); }
    ModelStorageColumns(const QSharedPointer<Lines>& rows, int minColumn, int maxColumn) { init(rows, minColumn, maxColumn); }
    ModelStorageColumns(const QSharedPointer<SpaceGrid>& grid, const QSet<int>& columns) { init(grid->rows(), columns); }
    ModelStorageColumns(const QSharedPointer<SpaceGrid>& grid, int minColumn, int maxColumn) { init(grid->rows(), minColumn, maxColumn); }

    ~ModelStorageColumns()
    {
        auto rows = m_rows.toStrongRef();
        if (rows)
            disconnect(rows.data(), &Lines::linesChanged, this, &ModelStorageColumns::onRowsChanged);
    }

protected:
    T valueImpl(const ItemID& item) const override
    {
        auto it = m_values.find(item.column);

        if (it == m_values.end() || item.row >= (*it).value().size())
            throw std::logic_error("Cannot get value");

        return (*it).value()[item.row];
    }

    bool setValueImpl(const ItemID& item, T value) override
    {
        auto it = m_values.find(item.column);

        if (it != m_values.end() && item.row < (*it).value().size())
        {
            (*it).value()[item.row] = value;
            return true;
        }
        else
        {
            return false;
        }
    }

private slots:
    void onRowsChanged(const Lines* lines, ChangeReason reason)
    {
        if (reason & ChangeReasonLinesCount)
        {
            resize();
        }
    }

private:
    void init(const QSharedPointer<Lines>& rows, const QSet<int>& columns)
    {
        m_rows = rows;

        QVector<StorageT> emptyValues;
        for (auto column: columns)
        {
            m_values[column] = emptyValues;
        }

        connect(rows.data(), &Lines::linesChanged, this, &ModelStorageColumns::onRowsChanged);
        resize();
    }

    void init(const QSharedPointer<Lines>& rows, int minColumn, int maxColumn)
    {
        m_rows = rows;

        QVector<StorageT> emptyValues;
        for (int column = minColumn; column <= maxColumn; ++column)
        {
            m_values[column] = emptyValues;
        }

        connect(rows.data(), &Lines::linesChanged, this, &ModelStorageColumns::onRowsChanged);
        resize();
    }

    void resize()
    {
        auto rows = m_rows.toStrongRef();
        Q_ASSERT(rows.data());

        for (auto& values: m_values)
        {
            values.value().resize(rows->count());
        }
    }

    QWeakPointer<Lines> m_rows;
    QMap<int, QVector<StorageT> > m_values;
};

template <typename T, typename StorageT = typename std::decay<T>::type>
class ModelStorageColumn: public ModelTyped<T>
{
public:
    ModelStorageColumn(const QSharedPointer<Lines>& rows)
        : m_rows(rows)
    {
        QObject::connect(rows.data(), &Lines::linesChanged, this, &ModelStorageColumn::onRowsChanged);
        resize();
    }

    ~ModelStorageColumn()
    {
        auto rows = m_rows.toStrongRef();
        if (rows)
            QObject::disconnect(rows.data(), &Lines::linesChanged, this, &ModelStorageColumn::onRowsChanged);
    }

    int size() const { return m_values.size(); }
    const QVector<StorageT>& values() const { return m_values; }
    void swapValues(QVector<StorageT>& values)
    {
        Q_ASSERT(m_values.size() == values.size());
        m_values.swap(values);
        emit modelChanged(this);
    }

    void setValueAll(T value)
    {
        m_values.fill(value);
        emit modelChanged(this);
    }

protected:
    T valueImpl(const ItemID& item) const override
    {
        if (item.row >= m_values.size())
            throw std::logic_error("Cannot return value");

        return m_values[item.row];
    }

    bool setValueImpl(const ItemID& item, T value) override
    {
        if (item.row < m_values.size())
        {
            m_values[item.row] = value;
            return true;
        }
        else
        {
            Q_ASSERT(false);
            return false;
        }
    }

private:
    void onRowsChanged(const Lines* /*rows*/, ChangeReason reason)
    {
        if (reason & ChangeReasonLinesCount)
            resize();
    }

    void resize()
    {
        auto rows = m_rows.toStrongRef();
        Q_ASSERT(rows);
        m_values.resize(rows->count());
    }

    QWeakPointer<Lines> m_rows;
    QVector<StorageT> m_values;
};

template <typename T, typename StorageT = typename std::decay<T>::type>
class ModelStorageRow: public ModelTyped<T>
{
public:
    ModelStorageRow(const QSharedPointer<Lines>& columns)
        : m_columns(columns)
    {
        connect(columns.data(), &Lines::linesChanged, this, ModelStorageRow::onColumnsChanged);
        resize();
    }

    ~ModelStorageRow()
    {
        auto columns = m_columns.toStrongRef();
        if (columns)
            disconnect(columns.data(), &Lines::linesChanged, this, ModelStorageRow::onColumnsChanged);
    }

    int size() const { return m_values.size(); }
    const QVector<StorageT>& values() const { return m_values; }
    void swapValues(QVector<StorageT>& values)
    {
        Q_ASSERT(m_values.size() == values.size());
        m_values.swap(values);
        emit modelChanged(this);
    }

    void setValueAll(T value)
    {
        m_values.fill(value);
        emit modelChanged(this);
    }

protected:
    T valueImpl(const ItemID& item) const override
    {
        if (item.column >= m_values.size())
            throw std::logic_error("Cannot return value");

        return m_values[item.column];
    }

    bool setValueImpl(const ItemID& item, T value) override
    {
        if (item.column < m_values.size())
        {
            m_values[item.column] = value;
            return true;
        }
        else
        {
            Q_ASSERT(false);
            return false;
        }
    }

private:
    void onColumnsChanged(const Lines* columns, ChangeReason reason)
    {
        if (reason & ChangeReasonLinesCount)
            resize();
    }

    void resize()
    {
        auto columns = m_columns.toStrongRef();
        Q_ASSERT(columns);
        m_values.resize(columns->count());
    }

    QWeakPointer<Lines> m_columns;
    QVector<StorageT> m_values;
};

template <typename T, typename StorageT = typename std::decay<T>::type>
class ModelStorageCells: public ModelTyped<T>
{
public:
    ModelStorageCells(const QSharedPointer<Range>& range = QSharedPointer<Range>())
        : m_range(range)
    {
    }

protected:
    T valueImpl(const ItemID& item) const override
    {
        Q_ASSERT(!m_range || m_range->hasItem(item));
        auto it = m_values.find(item);
        if (it != m_values.end())
            return (*it).value();
        else
            return T();
    }

    bool setValueImpl(const ItemID& item, T value) override
    {
        Q_ASSERT(!m_range || m_range->hasItem(item));
        m_values[item] = value;
        return true;
    }

private:
    QSharedPointer<Range> m_range;
    QMap<ItemID, StorageT> m_values;
};

template <typename T, typename StorageT = typename std::decay<T>::type>
class ModelStorageVector: public ModelTyped<T>
{
public:
    ModelStorageVector()
    {
    }

    ~ModelStorageVector()
    {
    }

    size_t size() const { return m_values.size(); }
    const QVector<StorageT>& values() const { return m_values; }
    void swapValues(QVector<StorageT>& values)
    {
        m_values.swap(values);
        emit this->modelChanged(this);
    }

    void setValueAll(T value, int size = -1)
    {
        m_values.fill(value, size);
        emit this->modelChanged(this);
    }

protected:
    T valueImpl(const ItemID& item) const override
    {
        if (item.row >= m_values.size())
            throw std::logic_error("Cannot return value");

        return m_values[item.row];
    }

    bool setValueImpl(const ItemID& item, T value) override
    {
        if (item.row < m_values.size())
        {
            m_values[item.row] = value;
            return true;
        }
        else
        {
            Q_ASSERT(false);
            return false;
        }
    }

private:
    QVector<StorageT> m_values;
};

} // end namespace Qi

#endif // QI_MODEL_STORE_H
