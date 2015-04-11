/*
   Copyright (c) 2008-1015 Alex Zhondin <qtinuum.team@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

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
