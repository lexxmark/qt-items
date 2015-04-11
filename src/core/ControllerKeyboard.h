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

#ifndef QI_CONTROLLER_KEYBOARD_H
#define QI_CONTROLLER_KEYBOARD_H

#include "QiAPI.h"
#include <QObject>
#include <QMouseEvent>
#include <functional>

namespace Qi
{

class QI_EXPORT ControllerKeyboard: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerKeyboard)

public:
    virtual ~ControllerKeyboard() {}

    virtual bool processKeyPress(QKeyEvent* /*event*/) { return false; }
    virtual bool processKeyRelease(QKeyEvent* /*event*/) { return false; }

    virtual void startCapturing() {}
    virtual void stopCapturing() {}

protected:
    ControllerKeyboard() {}
};

class QI_EXPORT ControllerKeyboardCallback: public ControllerKeyboard
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerKeyboardCallback)

public:
    ControllerKeyboardCallback() {}

    std::function<bool(QKeyEvent*)> onKeyPress;
    std::function<bool(QKeyEvent*)> onKeyRelease;

    bool processKeyPress(QKeyEvent* event) override;
    bool processKeyRelease(QKeyEvent* event) override;
};

class QI_EXPORT ControllerKeyboardChain: public ControllerKeyboard
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerKeyboardChain)

public:
    ControllerKeyboardChain();

    void clear();
    void append(const QSharedPointer<ControllerKeyboard>& controller);
    void prepend(const QSharedPointer<ControllerKeyboard>& controller);

    bool processKeyPress(QKeyEvent* event) override;
    bool processKeyRelease(QKeyEvent* event) override;

    void startCapturing() override;
    void stopCapturing() override;

private:
    QVector<QSharedPointer<ControllerKeyboard>> m_controllers;
    bool m_isInProcess;
};

} // end namespace Qi

#endif // QI_CONTROLLER_KEYBOARD_H
