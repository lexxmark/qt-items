#include "SpaceWidget_p.h"
#include "WidgetDriver.h"
#include "cache/space/CacheSpace.h"
#include "cache/CacheControllerMouse.h"
#include "core/ControllerKeyboard.h"

#include <QWidget>
#include <QToolTip>

namespace Qi
{

SpaceWidgetPrivate::SpaceWidgetPrivate(QWidget* owner, WidgetDriver* driver, const QSharedPointer<CacheSpace> &cacheSpace)
    : m_owner(owner),
      m_driver(driver),
      m_cacheSpace(cacheSpace),
      m_cacheControllers(new CacheControllerMouse(owner, driver, cacheSpace))
{
    Q_ASSERT(m_owner);
    Q_ASSERT(m_driver);
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

void SpaceWidgetPrivate::setControllerKeyboard(const QSharedPointer<ControllerKeyboard>& controllerKeyboard)
{
    m_controllerKeyboard = controllerKeyboard;
}

bool SpaceWidgetPrivate::ownerEvent(QEvent* event)
{
    bool processed = true;

    switch (event->type())
    {
    case QEvent::Resize:
    {
        QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(event);
        // resize window of the cache to occupy whole widget rect
        m_cacheSpace->setWindow(QRect(QPoint(0, 0), resizeEvent->size()));
    } break;

    case QEvent::Paint:
    {
        QPainter painter(m_owner);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::HighQualityAntialiasing);
        painter.setBackgroundMode(Qt::TransparentMode);
        // draw cache
        m_cacheSpace->draw(&painter, GuiContext(m_owner));
    } break;

    case QEvent::ToolTip:
    {
        QHelpEvent* helpEvent = static_cast<QHelpEvent *>(event);

        // show tooltip from the cache
        TooltipInfo tooltipInfo;
        if (m_cacheSpace->tooltipByPoint(helpEvent->pos(), tooltipInfo))
            QToolTip::showText(helpEvent->globalPos(), tooltipInfo.text, m_owner, tooltipInfo.rect);
        else
            QToolTip::hideText();
    } break;

    case QEvent::FocusIn:
    {
        if (m_controllerKeyboard)
            m_controllerKeyboard->startCapturing();
        // repaint owner
        m_owner->update();
    } break;

    case QEvent::FocusOut:
    {
        if (m_controllerKeyboard)
            m_controllerKeyboard->stopCapturing();
        // repaint owner
        m_owner->update();
    } break;

    case QEvent::KeyPress:
    {
        if (!m_cacheControllers->isCapturing() && m_controllerKeyboard)
            m_controllerKeyboard->processKeyPress(static_cast<QKeyEvent*>(event));
    } break;

    case QEvent::KeyRelease:
    {
        if (!m_cacheControllers->isCapturing() && m_controllerKeyboard)
            m_controllerKeyboard->processKeyRelease(static_cast<QKeyEvent*>(event));
    } break;

    default:
        processed = false;
        break;
    }

    // process event by controllers
    return m_cacheControllers->processEvent(event) || processed;
}

bool SpaceWidgetPrivate::doEdit(const CacheSpace& cacheSpace, const ItemID& visibleItem, const QKeyEvent* keyEvent)
{
    return m_cacheControllers->doEdit(cacheSpace, visibleItem, keyEvent, nullptr);
}

void SpaceWidgetPrivate::stopControllers()
{
    m_cacheControllers->stop();
}

void SpaceWidgetPrivate::resumeControllers()
{
    m_cacheControllers->resume();
}

void SpaceWidgetPrivate::onCacheSpaceChanged(const CacheSpace* cache, ChangeReason /*reason*/)
{
    Q_UNUSED(cache);
    Q_ASSERT(m_cacheSpace.data() == cache);
    // repaint owner widget
    m_owner->update();
}

} // end namespace Qi
