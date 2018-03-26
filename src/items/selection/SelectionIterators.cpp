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

namespace Qi
{

IdIteratorSelectedVisible::IdIteratorSelectedVisible(const ModelSelection& selection)
    : m_selection(selection),
      m_rows(nullptr),
      m_columns(nullptr)
{
    const auto& spaceGrid = m_selection.space();
    m_rows = spaceGrid.rows().data();
    m_columns = spaceGrid.columns().data();

    atFirst();
}

bool IdIteratorSelectedVisible::atFirstImpl()
{
    if (m_rows->isEmptyVisible() || m_columns->isEmptyVisible())
    {
        m_currentAbsId = GridID();
        return false;
    }

    m_currentVisibleId = GridID(0, 0);
    m_currentAbsId = GridID(m_rows->toAbsolute(m_currentVisibleId.row), m_columns->toAbsolute(m_currentVisibleId.column));

    if (m_selection.isItemSelected(m_currentAbsId))
        return true;

    return IdIteratorSelectedVisible::toNextImpl();
}

bool IdIteratorSelectedVisible::toNextImpl()
{
    if (!m_currentAbsId.isValid())
        return false;

    ++m_currentVisibleId.column;

    for (;m_currentVisibleId.row < m_rows->visibleCount(); ++m_currentVisibleId.row, m_currentVisibleId.column = 0)
    {
        for (;m_currentVisibleId.column < m_columns->visibleCount(); ++m_currentVisibleId.column)
        {
            m_currentAbsId = GridID(m_rows->toAbsolute(m_currentVisibleId.row), m_columns->toAbsolute(m_currentVisibleId.column));
            if (m_selection.isItemSelected(m_currentAbsId))
                return true;
        }
    }

    m_currentAbsId = GridID();
    return false;
}

IdIteratorSelectedVisibleByColumn::IdIteratorSelectedVisibleByColumn(const ModelSelection& selection, int absColumn)
    : m_selection(selection),
      m_rows(nullptr)
{
    auto spaceGrid = &m_selection.space();
    Q_ASSERT(spaceGrid);
    m_rows = spaceGrid->rows().data();

    m_currentAbsId.column = absColumn;
    if (absColumn != InvalidIndex)
        m_currentVisibleId.column = spaceGrid->columns()->toVisible(absColumn);

    atFirst();
}

bool IdIteratorSelectedVisibleByColumn::atFirstImpl()
{
    if (m_rows->isEmptyVisible() || m_currentVisibleId.column == InvalidIndex)
    {
        m_currentAbsId = GridID();
        return false;
    }

    m_currentVisibleId.row = 0;
    m_currentAbsId.row = m_rows->toAbsolute(m_currentVisibleId.row);

    if (m_selection.isItemSelected(m_currentAbsId))
        return true;

    return IdIteratorSelectedVisibleByColumn::toNextImpl();
}

bool IdIteratorSelectedVisibleByColumn::toNextImpl()
{
    if (!m_currentAbsId.isValid())
        return false;

    ++m_currentVisibleId.row;

    for (;m_currentVisibleId.row < m_rows->visibleCount(); ++m_currentVisibleId.row)
    {
        m_currentAbsId.row = m_rows->toAbsolute(m_currentVisibleId.row);
        if (m_selection.isItemSelected(m_currentAbsId))
            return true;
    }

    m_currentAbsId = GridID();
    return false;
}

} // end namespace Qi
