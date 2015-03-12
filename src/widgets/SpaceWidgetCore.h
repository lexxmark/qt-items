#ifndef QI_SPACE_WIDGET_CORE_H
#define QI_SPACE_WIDGET_CORE_H

#include "core/misc/ViewAuxiliary.h"

#include <QSharedPointer>
#include <QMetaObject>

class QWidget;
class QKeyEvent;

namespace Qi
{

class ItemID;
class CacheSpace;
class Space;
class CacheControllerMouse;
class ControllerKeyboard;

class SpaceWidgetCore
{
    Q_DISABLE_COPY(SpaceWidgetCore)

public:
    const CacheSpace& mainCacheSpace() const { return *m_mainCacheSpace; }
    const Space& mainSpace() const;

    CacheSpace& rMainCacheSpace() { return *m_mainCacheSpace; }
    Space& rMainSpace();

    const QSharedPointer<ControllerKeyboard>& controllerKeyboard() const { return m_controllerKeyboard; }
    void setControllerKeyboard(const QSharedPointer<ControllerKeyboard>& controllerKeyboard);
    void addControllerKeyboard(const QSharedPointer<ControllerKeyboard>& controllerKeyboard);

    // scrolls widget to make visibleItem fully visible
    void ensureVisible(const ItemID& visibleItem, const CacheSpace *cacheSpace, bool validateItem);
    // performs inplace editing for visibleItem
    bool doInplaceEdit(const ItemID& visibleItem, const CacheSpace *cacheSpace, const QKeyEvent* event);

protected:
    explicit SpaceWidgetCore(QWidget* owner);
    ~SpaceWidgetCore();

    bool initSpaceWidgetCore(const QSharedPointer<CacheSpace>& mainCacheSpace);

    bool processOwnerEvent(QEvent* event);

    void stopControllers();
    void resumeControllers();

    const GuiContext& guiContext() const { return m_guiContext; }

    // scrolls widget to make visibleItem fully visible
    virtual void ensureVisibleImpl(const ItemID& visibleItem, const CacheSpace *cacheSpace, bool validateItem) = 0;

private:
    void onCacheSpaceChanged(const CacheSpace* cache, ChangeReason reason);

    QWidget* m_owner;

    QSharedPointer<CacheSpace> m_mainCacheSpace;
    QScopedPointer<CacheControllerMouse> m_cacheControllers;
    QSharedPointer<ControllerKeyboard> m_controllerKeyboard;

    GuiContext m_guiContext;

    QMetaObject::Connection m_connection;

#if !defined(QT_NO_DEBUG)
    QPointer<QWidget> m_trackOwner;
#endif
};

} // end namespace Qi

#endif // QI_SPACE_WIDGET_P_H
