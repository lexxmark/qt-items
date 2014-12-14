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
    SpaceItem(const ItemID& item, SpaceHints hints = SpaceHintNone);
    ~SpaceItem();

    QSize size() const override { return m_size; }
    ItemID toAbsolute(const ItemID& visibleItem) const override { return visibleItem; }
    ItemID toVisible(const ItemID& absoluteItem) const override { return absoluteItem; }
    QRect itemRect(const ItemID& visibleItem) const override;

    void setSize(const QSize& size);

    const ItemID& item() const { return m_item; }
    void setItem(const ItemID& item);

private:
    ItemID m_item;
    QSize m_size;
};

} // end namespace Qi

#endif // QI_GRID_SPACE_H
