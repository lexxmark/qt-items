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

#include "SelectionIterators.h"
#include "space/SpaceGrid.h"

namespace Qi
{

ItemsIteratorSelectedVisible::ItemsIteratorSelectedVisible(const ModelSelection& selection)
    : m_selection(selection),
      m_rows(nullptr),
      m_columns(nullptr)
{
    auto spaceGrid = qobject_cast<const SpaceGrid*>(&m_selection.space());
    Q_ASSERT(spaceGrid);
    m_rows = spaceGrid->rows().data();
    m_columns = spaceGrid->columns().data();

    atFirst();
}

bool ItemsIteratorSelectedVisible::atFirstImpl()
{
    if (m_rows->isEmptyVisible() || m_columns->isEmptyVisible())
    {
        m_currentAbsItem = ItemID();
        return false;
    }

    m_currentVisibleItem = ItemID(0, 0);
    m_currentAbsItem = ItemID(m_rows->toAbsolute(m_currentVisibleItem.row), m_columns->toAbsolute(m_currentVisibleItem.column));

    if (m_selection.isItemSelected(m_currentAbsItem))
        return true;

    return ItemsIteratorSelectedVisible::toNextImpl();
}

bool ItemsIteratorSelectedVisible::toNextImpl()
{
    if (!m_currentAbsItem.isValid())
        return false;

    ++m_currentVisibleItem.column;

    for (;m_currentVisibleItem.row < m_rows->visibleCount(); ++m_currentVisibleItem.row, m_currentVisibleItem.column = 0)
    {
        for (;m_currentVisibleItem.column < m_columns->visibleCount(); ++m_currentVisibleItem.column)
        {
            m_currentAbsItem = ItemID(m_rows->toAbsolute(m_currentVisibleItem.row), m_columns->toAbsolute(m_currentVisibleItem.column));
            if (m_selection.isItemSelected(m_currentAbsItem))
                return true;
        }
    }

    m_currentAbsItem = ItemID();
    return false;
}

ItemsIteratorSelectedVisibleByColumn::ItemsIteratorSelectedVisibleByColumn(const ModelSelection& selection, int absColumn)
    : m_selection(selection),
      m_rows(nullptr)
{
    auto spaceGrid = qobject_cast<const SpaceGrid*>(&m_selection.space());
    Q_ASSERT(spaceGrid);
    m_rows = spaceGrid->rows().data();

    m_currentAbsItem.column = absColumn;
    if (absColumn != InvalidIndex)
        m_currentVisibleItem.column = spaceGrid->columns()->toVisible(absColumn);

    atFirst();
}

bool ItemsIteratorSelectedVisibleByColumn::atFirstImpl()
{
    if (m_rows->isEmptyVisible() || m_currentVisibleItem.column == InvalidIndex)
    {
        m_currentAbsItem = ItemID();
        return false;
    }

    m_currentVisibleItem.row = 0;
    m_currentAbsItem.row = m_rows->toAbsolute(m_currentVisibleItem.row);

    if (m_selection.isItemSelected(m_currentAbsItem))
        return true;

    return ItemsIteratorSelectedVisibleByColumn::toNextImpl();
}

bool ItemsIteratorSelectedVisibleByColumn::toNextImpl()
{
    if (!m_currentAbsItem.isValid())
        return false;

    ++m_currentVisibleItem.row;

    for (;m_currentVisibleItem.row < m_rows->visibleCount(); ++m_currentVisibleItem.row)
    {
        m_currentAbsItem.row = m_rows->toAbsolute(m_currentVisibleItem.row);
        if (m_selection.isItemSelected(m_currentAbsItem))
            return true;
    }

    m_currentAbsItem = ItemID();
    return false;
}

} // end namespace Qi
