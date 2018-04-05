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

#ifndef QI_RANGES_H
#define QI_RANGES_H

#include "core/Range.h"
#include <QSet>
#include <QVector>
#include <functional>

namespace Qi
{

class QI_EXPORT RangeCallback: public Range
{
    Q_OBJECT

public:
    RangeCallback(const std::function<bool(ID)>& hasItemCallback = nullptr)
        : hasItemCallback(hasItemCallback)
    {
    }

    std::function<bool(ID)> hasItemCallback;

protected:
    bool hasItemImpl(ID id) const override { return hasItemCallback ? hasItemCallback(id) : false; }
};

class QI_EXPORT RangeSelection: public Range
{
    Q_OBJECT

public:
    RangeSelection() {}
    RangeSelection(const RangeSelection& other): Range() { *this = other; }

    RangeSelection& operator=(const RangeSelection& other);

    bool isEmpty() const { return m_ranges.isEmpty(); }
    void clear();
    void addRange(SharedPtr<Range> range, bool exclude);

protected:
    bool hasItemImpl(ID id) const override;

private:
    struct RangeInfo
    {
        SharedPtr<Range> range;
        bool exclude;
    };

    QVector<RangeInfo> m_ranges;
};

class QI_EXPORT RangeNone: public Range
{
    Q_OBJECT
    
public:
    RangeNone();
    
protected:
    bool hasItemImpl(ID id) const override;
};
QI_EXPORT SharedPtr<RangeNone> makeRangeNone();

class QI_EXPORT RangeAll: public Range
{
    Q_OBJECT
    
public:
    RangeAll();
    
protected:
    bool hasItemImpl(ID id) const override;
};
QI_EXPORT SharedPtr<RangeAll> makeRangeAll();

class QI_EXPORT RangeID: public Range
{
    Q_OBJECT

public:
    explicit RangeID(ID id);

    ID id() const { return m_id; }
    void setId(ID id);

protected:
    bool hasItemImpl(ID id) const override;

private:
    ID m_id;
};
QI_EXPORT SharedPtr<RangeID> makeRangeID(ID id);

} // end namespace Qi

#endif // QI_RANGES_H
