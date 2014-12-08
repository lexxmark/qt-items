#ifndef QI_CONTROLLER_MOUSE_H
#define QI_CONTROLLER_MOUSE_H

#include "ItemID.h"
#include <QObject>

#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QContextMenuEvent>
#include <QFocusEvent>

class QWidget;

namespace Qi
{

class WidgetDriver;
class ControllerMouse;
class CacheContext;
class CacheSpace;

enum ControllerMousePriority
{
    ControllerMousePriorityNormal = 1, // normal priority
    ControllerMousePriorityBackground = 0, // low priority
    ControllerMousePriorityOverlay = 2 // high priority
};

class QI_EXPORT ControllerContext
{
public:
    QWidget* widget;
    WidgetDriver* widgetDriver;
    QPoint point;

    virtual void notifyStartCapturing(ControllerMouse& controller) = 0;
    virtual void notifyStopCapturing(ControllerMouse& controller) = 0;

protected:
    // constructor/destructor are accessible from
    // derived classes only
    ControllerContext(QWidget* widget, WidgetDriver* widgetDriver)
        : widget(widget), widgetDriver(widgetDriver), point(0, 0)
    {}

    ~ControllerContext()
    {}
};

class QI_EXPORT ControllerMouse: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerMouse)

public:
    virtual ~ControllerMouse();

    ControllerMousePriority priority() const { return m_priority; }
    bool isCapturing() const { return m_state == ControllerMouseStateCapturing; }
    bool isActive() const { return m_state != ControllerMouseStateInactive; }
    ItemID activeItem() const;

    void tryActivate(QVector<ControllerMouse*>& activatedControllers, const ControllerContext& context, const CacheContext& cache, const CacheSpace& cacheSpace);
    void deactivate();
    void stopCapturing();

    virtual void processPaint(QPaintEvent* /*event*/) {}
    virtual void processKillFocus(QFocusEvent* /*event*/) {}
    virtual bool processSetCursor() { return false; }
    virtual bool processLButtonDown(QMouseEvent* /*event*/) { return false; }
    virtual bool processLButtonUp(QMouseEvent* /*event*/) { return false; }
    virtual bool processLButtonDblClick(QMouseEvent* /*event*/) { return false; }
    virtual bool processMouseMove(QMouseEvent* /*event*/) { return false; }
    virtual bool processContextMenu(QContextMenuEvent* /*event*/) { return false; }

    bool acceptEdit(const ItemID& item, const CacheSpace& cacheSpace, const QKeyEvent* keyEvent = nullptr) const;
    void doEdit(QVector<ControllerMouse*>& activatedControllers, const ControllerContext& context, const CacheContext& cache, const CacheSpace& cacheSpace, const QKeyEvent* keyEvent = nullptr);

    virtual void updateEditLayout() {}

protected:
    ControllerMouse(ControllerMousePriority priority = ControllerMousePriorityNormal);

    enum ControllerMouseState
    {
        ControllerMouseStateInactive,
        ControllerMouseStateActive,
        ControllerMouseStateDoubleActive,
        ControllerMouseStateCapturing
    };
    ControllerMouseState state() const { return m_state; }

    class QI_EXPORT ActivationInfo
    {
    public:
        ActivationInfo(const ControllerContext& context, const CacheContext& cache, const CacheSpace& cacheSpace);

        const ItemID& item() const;
        const QPoint& point() const { return context.point; }

        const ControllerContext& context;
        const CacheContext& cache;
        const CacheSpace& cacheSpace;
    };

    class QI_EXPORT ActivationState
    {
    public:
        explicit ActivationState(const ActivationInfo& info);

        const QPoint& point() const { return context.point; }
        ItemID visibleItem() const;

        const ControllerContext& context;
        const ItemID item;
        const CacheSpace& cacheSpace;

        // these members can be out of sync
        // for example when grid "auto-scrolls" and user drags mouse
        // near the grid edge
        const QRect itemRect;
        const QRect viewRect;
        const QRect* visibleRect;
    };

    void startCapturing();
    const ActivationState& activationState() const;

    virtual void tryActivateImpl(QVector<ControllerMouse*>& activatedControllers, const ActivationInfo& activationInfo);
    virtual bool acceptImpl(const ActivationInfo& /*activationInfo*/) const { return true; }
    virtual void activateImpl(const ActivationInfo& activationInfo);
    virtual bool needDeactivateImpl(const ActivationInfo& activationInfo) const;
    virtual void deactivateImpl();
    virtual void startCapturingImpl();
    virtual void stopCapturingImpl();

    virtual bool acceptEditImpl(const ItemID& /*item*/, const CacheSpace& /*cacheSpace*/, const QKeyEvent* /*keyEvent*/) const { return false; }
    virtual void doEditImpl(const QKeyEvent* /*keyEvent*/) {}

private:
    void activate(const ActivationInfo& activationInfo);

    ControllerMousePriority m_priority;
    ControllerMouseState m_state;
    QScopedPointer<ActivationState> m_activationState;
};

} // end namespace Qi

#endif // QI_CONTROLLER_MOUSE_H
