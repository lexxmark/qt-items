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

#ifndef QI_RANGE_H
#define QI_RANGE_H

#include <QObject>
#include "ItemID.h"

namespace Qi
{

class QI_EXPORT Range: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Range)

public:
    Range();
    virtual ~Range();

    bool hasItem(const ItemID& item) const { return hasItemImpl(item); }
    bool hasItem(int row, int column) const { return hasItem(ItemID(row, column)); }

    bool hasRow(int row) const { return hasRowImpl(row); }
    bool hasColumn(int column) const { return hasColumnImpl(column); }

signals:
    void rangeChanged(const Range*, ChangeReason);

protected:
    // should return true if item is included in the range and false otherwise
    virtual bool hasItemImpl(const ItemID &item) const { return hasRow(item.row) && hasColumn(item.column); }
    // should return true if row intersets with the range and false otherwise
    virtual bool hasRowImpl(int row) const = 0;
    // should return true if column intersets with the range and false otherwise
    virtual bool hasColumnImpl(int column) const = 0;
};

} // end namespace Qi

#endif // QI_RANGE_H
