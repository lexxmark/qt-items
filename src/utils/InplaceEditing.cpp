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

#include "InplaceEditing.h"
#include <QScopedPointer>
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>

namespace Qi
{

InplaceEdit& inplaceEdit()
{
    static InplaceEdit theInplaceEdit;
    return theInplaceEdit;
}

InplaceEdit::InplaceEdit()
    : m_inplaceEditor(nullptr)
{
}

bool InplaceEdit::start(QWidget* editor)
{
    if (!editor)
        return false;

    if (m_inplaceEditor)
    {
        Q_ASSERT(false);
        stop();
    }

    QCoreApplication* app = QCoreApplication::instance();
    if (!app)
    {
        Q_ASSERT(false);
        return false;
    }

    m_inplaceEditor = editor;

    // show editor
    if (!m_inplaceEditor->isVisible())
        m_inplaceEditor->show();

    // move focus to editor
    if (QApplication::focusWidget() != m_inplaceEditor->focusWidget())
        m_inplaceEditor->setFocus();

    // connect to editor destroyed signal
    QObject::connect(m_inplaceEditor, &QObject::destroyed, this, &InplaceEdit::onEditorDestroyed);

    // install application event filter
    app->installEventFilter(this);

    return true;
}

bool InplaceEdit::stop()
{
    if (!m_inplaceEditor)
        return false;

    auto inplaceEditor = m_inplaceEditor;

    // finalize
    disconnect(m_inplaceEditor, &QObject::destroyed, this, &InplaceEdit::onEditorDestroyed);
    onEditorDestroyed(m_inplaceEditor);
    Q_ASSERT(!m_inplaceEditor);

    // delete widget later
    // because stop() can be called from widget's signal
    // so we cannot delete widget right here
    inplaceEditor->hide();
    inplaceEditor->deleteLater();

    return true;
}

static bool hasParent(QObject* child, QObject* parent)
{
    if (!child)
        return false;

    if (child == parent)
        return true;

    return hasParent(child->parent(), parent);
}

bool InplaceEdit::eventFilter(QObject* watched, QEvent* event)
{
    Q_ASSERT(m_inplaceEditor);

    if (!event)
        return false;

    // try handle by base class
    if (QObject::eventFilter(watched, event))
        return true;

    if (event->type() == QEvent::FocusIn)
    {
        // if editor looses focus -> close it
        if (!hasParent(QApplication::focusObject(), m_inplaceEditor))
            stop();

        return false;
    }

    return false;
}

void InplaceEdit::onEditorDestroyed(QObject* object)
{
    Q_UNUSED(object);
    Q_ASSERT(object == m_inplaceEditor);

    QCoreApplication::instance()->removeEventFilter(this);
    m_inplaceEditor = nullptr;
}

} // end namespace Qi
