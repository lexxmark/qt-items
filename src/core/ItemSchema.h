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
    SharedPtr<Layout> layout;
    SharedPtr<View> view;

    ViewSchema() {}
    ViewSchema(SharedPtr<Layout> layout, SharedPtr<View> view)
        : layout(std::move(layout)),
          view(std::move(view))
    {}

    bool isValid() const { return layout && view; }
};

struct QI_EXPORT ItemSchema
{
    SharedPtr<Range> range;
    SharedPtr<Layout> layout;
    SharedPtr<View> view;

    ItemSchema() {}
    ItemSchema(SharedPtr<Range> range, SharedPtr<Layout> layout, SharedPtr<View> view)
        : range(std::move(range)),
          layout(std::move(layout)),
          view(std::move(view))
    {}
};

} // end namespace Qi

#endif // QI_ITEMS_SCHEMA_H
