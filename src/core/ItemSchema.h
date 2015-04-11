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

#ifndef QI_ITEMS_SCHEMA_H
#define QI_ITEMS_SCHEMA_H

#include "Layout.h"
#include "Range.h"

namespace Qi
{

struct QI_EXPORT ViewSchema
{
    QSharedPointer<Layout> layout;
    QSharedPointer<View> view;

    ViewSchema() {}
    ViewSchema(QSharedPointer<Layout> layout, QSharedPointer<View> view)
        : layout(layout),
          view(view)
    {}

    bool isValid() const { return layout && view; }
};

struct QI_EXPORT ItemSchema
{
    QSharedPointer<Range> range;
    QSharedPointer<Layout> layout;
    QSharedPointer<View> view;

    ItemSchema() {}
    ItemSchema(QSharedPointer<Range> range, QSharedPointer<Layout> layout, QSharedPointer<View> view)
        : range(range),
          layout(layout),
          view(view)
    {}
};

} // end namespace Qi

#endif // QI_ITEMS_SCHEMA_H
