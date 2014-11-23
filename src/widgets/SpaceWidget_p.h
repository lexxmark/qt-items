#ifndef QI_SPACE_WIDGET_P_H
#define QI_SPACE_WIDGET_P_H

#include "QiAPI.h"

#include <QSharedPointer>
#include <QMetaObject>

class QWidget;

namespace Qi
{

class CacheSpace;
class CacheControllerMouse;

class SpaceWidgetPrivate
{
    Q_DISABLE_COPY(SpaceWidgetPrivate)

public:
    explicit SpaceWidgetPrivate(QWidget* owner, const QSharedPointer<CacheSpace>& cacheSpace);
    ~SpaceWidgetPrivate();

    const CacheSpace& cacheSpace() const { return *m_cacheSpace; }
    
    bool ownerEvent(QEvent* event);

    void stopControllers();
    void resumeControllers();

private:
    void onCacheSpaceChanged(const CacheSpace* cache, ChangeReason reason);

    QWidget* m_owner;

    QSharedPointer<CacheSpace> m_cacheSpace;
    QScopedPointer<CacheControllerMouse> m_cacheControllers;

    QMetaObject::Connection m_connection;

#if !defined(QT_NO_DEBUG)
    QPointer<QWidget> m_trackOwner;
#endif
};

} // end namespace Qi

#endif // QI_SPACE_WIDGET_P_H
