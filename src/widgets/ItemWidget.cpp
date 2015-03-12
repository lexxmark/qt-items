#include "ItemWidget.h"
#include "cache/space/CacheSpaceItem.h"
#include "cache/CacheItem.h"
#include <QResizeEvent>

namespace Qi
{

ItemWidget::ItemWidget(QWidget* parent)
    : SpaceWidgetAbstract(parent),
      m_syncSpaceSizeWithContent(true)
{

    m_space = QSharedPointer<SpaceItem>::create(ItemID(0, 0));
    initSpaceWidgetCore(QSharedPointer<CacheSpaceItem>::create(m_space));

    connect(m_space.data(), &Space::spaceChanged, this, &ItemWidget::onSpaceChanged);
}

ItemWidget::~ItemWidget()
{
    disconnect(m_space.data(), &Space::spaceChanged, this, &ItemWidget::onSpaceChanged);
}

void ItemWidget::syncSpaceSizeWithContent(bool enable)
{
    if (m_syncSpaceSizeWithContent == enable)
        return;

    m_syncSpaceSizeWithContent = enable;

    if (m_syncSpaceSizeWithContent)
    {
        // shrink space size to fit item content
        auto cacheItem = mainCacheSpace().cacheItem(m_space->item());
        Q_ASSERT(cacheItem);
        if (!cacheItem)
            m_space->setSize(QSize());
        else
            m_space->setSize(cacheItem->calculateItemSize(guiContext()));
    }
    else
    {
        // sync space size with widget size
        m_space->setSize(size());
    }
}

QSize ItemWidget::sizeHint() const
{
    return m_space->size();
}

QSize ItemWidget::minimumSizeHint() const
{
    if (m_syncSpaceSizeWithContent)
        return m_space->size();
    else
        return SpaceWidgetAbstract::minimumSizeHint();
}

bool ItemWidget::event(QEvent* e)
{
    if (!m_syncSpaceSizeWithContent && e->type() == QEvent::Resize)
    {
        QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(e);
        // sync space size with widget size
        m_space->setSize(resizeEvent->size());
    }

    return SpaceWidgetAbstract::event(e);
}

void ItemWidget::onSpaceChanged(const Space* space, ChangeReason reason)
{
    Q_UNUSED(space);
    Q_ASSERT(m_space.data() == space);

    if (reason & ChangeReasonSpaceItemsStructure)
    {
        // shrink space size to fit item content
        if (m_syncSpaceSizeWithContent)
        {
            auto cacheItem = mainCacheSpace().cacheItem(m_space->item());
            Q_ASSERT(cacheItem);
            if (!cacheItem)
                m_space->setSize(QSize());
            else
                m_space->setSize(cacheItem->calculateItemSize(guiContext()));
        }
    }

    if (reason & ChangeReasonSpaceStructure)
    {
        // notify system to recalculate layout
        updateGeometry();
    }
}

} // end namespace Qi
