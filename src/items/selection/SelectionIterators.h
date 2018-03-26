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

#ifndef QI_SELECTION_ITERATORS_H
#define QI_SELECTION_ITERATORS_H

#include "core/ItemsIterator.h"
#include "Selection.h"

namespace Qi
{

class Lines;

class QI_EXPORT IdIteratorSelectedVisible: public IdIterator
{
public:
    explicit IdIteratorSelectedVisible(const ModelSelection& selection);

    GridID visibleId() const { return m_currentVisibleId; }

protected:
    ID idImpl() const override { return ID(m_currentAbsId); }
    bool atFirstImpl() override;
    bool toNextImpl() override;

private:
    const ModelSelection& m_selection;
    const Lines* m_rows;
    const Lines* m_columns;
    GridID m_currentVisibleId;
    GridID m_currentAbsId;
};

class QI_EXPORT IdIteratorSelectedVisibleByColumn: public IdIterator
{
public:
    explicit IdIteratorSelectedVisibleByColumn(const ModelSelection& selection, int absColumn = 0);

    GridID visibleId() const { return m_currentVisibleId; }

protected:
    ID idImpl() const override { return ID(m_currentAbsId); }
    bool atFirstImpl() override;
    bool toNextImpl() override;

private:
    const ModelSelection& m_selection;
    const Lines* m_rows;
    GridID m_currentVisibleId;
    GridID m_currentAbsId;
};


} // end namespace Qi

#endif // QI_SELECTION_ITERATORS_H
