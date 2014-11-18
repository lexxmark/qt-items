#include "SpaceItem.h"

namespace Qi
{

SpaceItem::SpaceItem(const ItemID& item, SpaceHints hints)
    : Space(hints),
      m_item(item)
{
}

SpaceItem::~SpaceItem()
{
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
