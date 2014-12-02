#ifndef QI_SPACE_WIDGET_P_H
#define QI_SPACE_WIDGET_P_H

#include "QiAPI.h"

#include <QSharedPointer>
#include <QMetaObject>

class QWidget;
class QKeyEvent;

namespace Qi
{

class ItemID;
class WidgetDriver;
class CacheSpace;
class CacheControllerMouse;
class ControllerKeyboard;

class SpaceWidgetPrivate
{
    Q_DISABLE_COPY(SpaceWidgetPrivate)

public:
    explicit SpaceWidgetPrivate(QWidget* owner, WidgetDriver* driver, const QSharedPointer<CacheSpace>& cacheSpace);
    ~SpaceWidgetPrivate();

    const CacheSpace& cacheSpace() const { return *m_cacheSpace; }

    const QSharedPointer<ControllerKeyboard>& controllerKeyboard() const { return m_controllerKeyboard; }
    void setControllerKeyboard(const QSharedPointer<ControllerKeyboard>& controllerKeyboard);
    
    bool ownerEvent(QEvent* event);
    bool doEdit(const CacheSpace& cacheSpace, const ItemID& visibleItem, const QKeyEvent* keyEvent);

    void stopControllers();
    void resumeControllers();

private:
    void onCacheSpaceChanged(const CacheSpace* cache, ChangeReason reason);

    QWidget* m_owner;
    WidgetDriver* m_driver;

    QSharedPointer<CacheSpace> m_cacheSpace;
    QScopedPointer<CacheControllerMouse> m_cacheControllers;
    QSharedPointer<ControllerKeyboard> m_controllerKeyboard;

    QMetaObject::Connection m_connection;

#if !defined(QT_NO_DEBUG)
    QPointer<QWidget> m_trackOwner;
#endif
};

} // end namespace Qi

#endif // QI_SPACE_WIDGET_P_H
