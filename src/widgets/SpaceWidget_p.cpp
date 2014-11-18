#include "SpaceWidget_p.h"
#include "cache/space/CacheSpace.h"
#include "cache/CacheControllerMouse.h"

#include <QWidget>
#include <QToolTip>

namespace Qi
{

SpaceWidgetPrivate::SpaceWidgetPrivate(QWidget* owner, const QSharedPointer<CacheSpace> &cacheSpace)
    : m_owner(owner),
      m_cacheSpace(cacheSpace),
      m_cacheControllers(new CacheControllerMouse(owner, cacheSpace))
{
    Q_ASSERT(m_owner);
    Q_ASSERT(m_cacheSpace);

#if !defined(QT_NO_DEBUG)
    m_trackOwner = m_owner;
#endif

    m_connection = QObject::connect(m_cacheSpace.data(), &CacheSpace::cacheChanged, [this](const CacheSpace* cache, ChangeReason reason) {
        onCacheSpaceChanged(cache, reason);
    });
}

SpaceWidgetPrivate::~SpaceWidgetPrivate()
{
#if !defined(QT_NO_DEBUG)
    // owner should exist
    Q_ASSERT(m_trackOwner);
#endif

    QObject::disconnect(m_connection);
}

bool SpaceWidgetPrivate::ownerEvent(QEvent* event)
{
    switch (event->type())
    {
        case QEvent::Resize:
        {
            QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(event);
            // resize window of the cache
            m_cacheSpace->setWindow(QRect(QPoint(0, 0), resizeEvent->size()));
            break;
        }

        case QEvent::Paint:
        {
            QPainter painter(m_owner);
            painter.setRenderHint(QPainter::Antialiasing);
            // draw cache
            m_cacheSpace->draw(&painter, GuiContext(m_owner));
        }

        case QEvent::ToolTip:
        {
            QHelpEvent* helpEvent = static_cast<QHelpEvent *>(event);

            // show tooltip from the cache
            TooltipInfo tooltipInfo;
            if (m_cacheSpace->tooltipByPoint(helpEvent->pos(), tooltipInfo))
                QToolTip::showText(helpEvent->globalPos(), tooltipInfo.text, m_owner, tooltipInfo.rect);
            else
                QToolTip::hideText();
        }

        default:
            break;
    }

    // process event by controllers
    return m_cacheControllers->processEvent(event);
}

void SpaceWidgetPrivate::onCacheSpaceChanged(const CacheSpace* cache, ChangeReason reason)
{
    Q_ASSERT(m_cacheSpace.data() == cache);
    // repaint owner widget
    m_owner->update();
}

} // end namespace Qi
