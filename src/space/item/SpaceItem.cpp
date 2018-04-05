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

SpaceItem::SpaceItem(ID id)
    : m_id(id)
{
}

SpaceItem::~SpaceItem()
{
}

QRect SpaceItem::itemRect(ID visibleId) const
{
    Q_UNUSED(visibleId);
    Q_ASSERT(m_id == visibleId);
    return QRect(QPoint(0, 0), m_size);
}

SharedPtr<CacheItemFactory> SpaceItem::createCacheItemFactory(ViewApplicationMask viewApplicationMask) const
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

void SpaceItem::setId(ID id)
{
    if (m_id == id)
        return;

    m_id = id;

    emit spaceChanged(this, ChangeReasonSpaceStructure);
}

} // end namespace Qi
