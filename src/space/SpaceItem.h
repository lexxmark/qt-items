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

#ifndef QI_SPACE_ITEM_H
#define QI_SPACE_ITEM_H

#include "Space.h"

namespace Qi
{

class QI_EXPORT SpaceItem: public Space
{
    Q_OBJECT
    Q_DISABLE_COPY(SpaceItem)

public:
    SpaceItem(const ItemID& item);
    ~SpaceItem();

    QSize size() const override { return m_size; }
    ItemID toAbsolute(const ItemID& visibleItem) const override { return visibleItem; }
    ItemID toVisible(const ItemID& absoluteItem) const override { return absoluteItem; }
    QRect itemRect(const ItemID& visibleItem) const override;
    QSharedPointer<CacheItemFactory> createCacheItemFactory(ViewApplicationMask viewApplicationMask = ViewApplicationNone) const override;

    void setSize(const QSize& size);

    const ItemID& item() const { return m_item; }
    void setItem(const ItemID& item);

private:
    ItemID m_item;
    QSize m_size;
};

} // end namespace Qi

#endif // QI_GRID_SPACE_H
