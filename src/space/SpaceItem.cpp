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

#include "SpaceItem.h"
#include "cache/CacheItemFactory.h"

namespace Qi
{

SpaceItem::SpaceItem(const ItemID& item)
    : m_item(item)
{
}

SpaceItem::~SpaceItem()
{
}

QRect SpaceItem::itemRect(const ItemID& visibleItem) const
{
    Q_UNUSED(visibleItem);
    Q_ASSERT(m_item == visibleItem);
    return QRect(QPoint(0, 0), m_size);
}

QSharedPointer<CacheItemFactory> SpaceItem::createCacheItemFactory(ViewApplicationMask viewApplicationMask) const
{
    return createCacheItemFactoryItem(*this, viewApplicationMask);
}

void SpaceItem::setSize(const QSize& size)
{
    if (m_size == size)
        return;

    m_size = size;

    emit spaceChanged(this, ChangeReasonSpaceStructure);
}

void SpaceItem::setItem(const ItemID& item)
{
    if (m_item == item)
        return;

    m_item = item;

    emit spaceChanged(this, ChangeReasonSpaceStructure);
}

} // end namespace Qi
