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

QRect SpaceItem::itemRect(const ItemID& visibleItem) const
{
    Q_UNUSED(visibleItem);
    Q_ASSERT(m_item == visibleItem);
    return QRect(QPoint(0, 0), m_size);
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
