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

#include "Ranges.h"

namespace Qi
{

RangeSelection& RangeSelection::operator=(const RangeSelection& other)
{
    m_ranges = other.m_ranges;
    emit rangeChanged(this, ChangeReasonRange);

    return *this;
}

void RangeSelection::clear()
{
    m_ranges.clear();
    emit rangeChanged(this, ChangeReasonRange);
}

void RangeSelection::addRange(const QSharedPointer<Range>& range, bool exclude)
{
    RangeInfo info = { range, exclude };
    m_ranges.append(info);
    emit rangeChanged(this, ChangeReasonRange);
}

bool RangeSelection::hasItemImpl(ID id) const
{
    bool excluded = true;
    for (const auto& range: m_ranges)
    {
        if (range.range->hasItem(id))
        {
            excluded = range.exclude;
        }
    }

    return !excluded;
}

RangeNone::RangeNone()
{
}

bool RangeNone::hasItemImpl(ID /*id*/) const
{
    return false;
}

QSharedPointer<RangeNone> makeRangeNone()
{
    return QSharedPointer<RangeNone>::create();
}

RangeAll::RangeAll()
{
}

bool RangeAll::hasItemImpl(ID /*id*/) const
{
    return true;
}

QSharedPointer<RangeAll> makeRangeAll()
{
    return QSharedPointer<RangeAll>::create();
}

RangeID::RangeID(ID id)
    : m_id(id)
{
}

void RangeID::setId(ID id)
{
    if (m_id != id)
    {
        m_id = id;
        emit rangeChanged(this, ChangeReasonRange);
    }
}

bool RangeID::hasItemImpl(ID id) const
{
    return m_id == id;
}

QSharedPointer<RangeID> makeRangeID(ID id)
{
    return QSharedPointer<RangeID>::create(id);
}

} // end namespace Qi
