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

#ifndef QI_ITEM_ID_H
#define QI_ITEM_ID_H

#include "QiAPI.h"
#include <QHash>

namespace Qi
{

class QI_EXPORT ItemID
{
public:
    int row;
    int column;

    ItemID()
        : row(InvalidIndex), column(InvalidIndex)
    {}

    ItemID(int row, int column)
        : row(row), column(column)
    {}

    bool isValid() const { return row >= 0 && column >= 0; }
    
    void swap(ItemID& other)
    {
        std::swap(row, other.row);
        std::swap(column, other.column);
    }
};

inline bool operator==(ItemID left, ItemID right)
{
    return (left.row == right.row) && (left.column == right.column);
}

inline bool operator!=(ItemID left, ItemID right)
{
    return !(left == right);
}

inline ItemID operator+(ItemID left, ItemID right)
{
    return ItemID(left.row+right.row, left.column+right.column);
}

inline ItemID operator-(ItemID left, ItemID right)
{
    Q_ASSERT(left.row >= right.row);
    Q_ASSERT(left.column >= right.column);

    return ItemID(left.row-right.row, left.column-right.column);
}

// std::set/std::map support
inline bool operator<(ItemID left, ItemID right)
{
    if (left.row == right.row)
        return left.column < right.column;
    else
        return left.row < right.row;
}

// QSet/QMap support
inline uint qHash(ItemID key)
{
    return qHash(QPair<int, int>(key.row, key.column));
}

} // end namespace Qi

#endif // QI_ITEM_ID_H
