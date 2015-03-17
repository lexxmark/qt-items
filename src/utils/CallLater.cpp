#include "CallLater.h"
#include <QEvent>
#include <QCoreApplication>
#include <QDebug>

namespace Qi
{

const QEvent::Type QCallEventType = QEvent::Type(QEvent::User+1);

class QCallEvent: public QEvent
{
public:
    QCallEvent(QObject* owner, const std::function<void()>& callback)
        : QEvent(QCallEventType),
          m_owner(owner),
          m_callback(callback)
    {
        Q_ASSERT(m_owner);
        Q_ASSERT(m_callback);
    }

    void process() const
    {
        if (m_owner.isNull())
            return;

        m_callback();
    }

private:
    QPointer<QObject> m_owner;
    std::function<void()> m_callback;
};

class CallManager: public QObject
{
    Q_DISABLE_COPY(CallManager)

public:
    CallManager()
    {
    }

    bool event(QEvent* event) override
    {
        if (event->type() == QCallEventType)
        {
            QCallEvent* callEvent = (QCallEvent*)event;
            callEvent->process();
        }

        return true;
    }
};

void callLater(QObject* owner, const std::function<void()>& callback)
{
    static CallManager callManager;

    QCoreApplication::instance()->postEvent(&callManager, new QCallEvent(owner, callback));
}

} // end namespace Qi
