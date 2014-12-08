#include "ItemWidget.h"
#include "cache/CacheItem.h"
#include "SpaceWidget_p.h"
#include <QResizeEvent>

namespace Qi
{

ItemWidget::ItemWidget(QWidget* parent)
    : QWidget(parent),
      m_syncSpaceSizeWithContent(true)
{
    m_space = QSharedPointer<SpaceItem>::create(ItemID(0, 0));
    auto cacheSpace = QSharedPointer<CacheSpaceItem>::create(m_space);
    m_impl.reset(new SpaceWidgetPrivate(this, this, cacheSpace));

    connect(m_space.data(), &Space::spaceChanged, this, &ItemWidget::onSpaceChanged);

    // enable tracking mouse moves
    setMouseTracking(true);
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
        auto cacheItem = m_impl->cacheSpace().cacheItem(m_space->item());
        Q_ASSERT(cacheItem);
        if (!cacheItem)
            m_space->setSize(QSize());
        else
            m_space->setSize(cacheItem->calculateItemSize(GuiContext(this)) + QSize(0, 0));
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
        return QWidget::minimumSizeHint();
}

bool ItemWidget::event(QEvent* e)
{
    bool result = QWidget::event(e);

    if (!m_syncSpaceSizeWithContent && e->type() == QEvent::Resize)
    {
        QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(e);
        // sync space size with widget size
        m_space->setSize(resizeEvent->size());
    }

    result |= m_impl->ownerEvent(e);
    return result;
}

void ItemWidget::onSpaceChanged(const Space* space, ChangeReason reason)
{
    Q_ASSERT(m_space.data() == space);

    if (reason & ChangeReasonSpaceItemsStructure)
    {
        // shrink space size to fit item content
        if (m_syncSpaceSizeWithContent)
        {
            auto cacheItem = m_impl->cacheSpace().cacheItem(m_space->item());
            Q_ASSERT(cacheItem);
            if (!cacheItem)
                m_space->setSize(QSize());
            else
                m_space->setSize(cacheItem->calculateItemSize(GuiContext(this)) + QSize(0, 0));
        }
    }

    if (reason & ChangeReasonSpaceStructure)
    {
        // notify system to recalculate layout
        updateGeometry();
    }
}

void ItemWidget::ensureVisible(const ItemID& visibleItem, const CacheSpace *cacheSpace, bool /*validateItem*/)
{
    Q_ASSERT(visibleItem == m_space->item());
    Q_ASSERT(&m_impl->cacheSpace() == cacheSpace);
    // not applicable
}

bool ItemWidget::doEdit(const ItemID& visibleItem, const CacheSpace *cacheSpace, const QKeyEvent* event)
{
    Q_ASSERT(visibleItem == m_space->item());
    Q_ASSERT(&m_impl->cacheSpace() == cacheSpace);
    return m_impl->doEdit(*cacheSpace, visibleItem, event);
}

} // end namespace Qi
