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
#include "ID.h"

namespace Qi
{

class QI_EXPORT Range: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Range)

public:
    virtual ~Range() = default;

    bool hasItem(ID id) const { return hasItemImpl(id); }

signals:
    void rangeChanged(const Range*, ChangeReason);

protected:
    Range() = default;

    // should return true if item is included in the range and false otherwise
    virtual bool hasItemImpl(ID id) const = 0;
};

} // end namespace Qi

#endif // QI_RANGE_H
