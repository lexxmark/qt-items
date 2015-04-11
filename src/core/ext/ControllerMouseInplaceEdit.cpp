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

#include "ControllerMouseInplaceEdit.h"
#include "cache/space/CacheSpace.h"
#include "utils/InplaceEditing.h"

#include <QWidget>

namespace Qi
{

ControllerMouseInplaceEdit::ControllerMouseInplaceEdit(ControllerMousePriority priority)
    : ControllerMouse(priority),
      m_editBySingleClick(false),
      m_layoutMode(InplaceEditLayoutModeItem),
      m_inplaceEditor(nullptr)
{
}

bool ControllerMouseInplaceEdit::processLButtonDown(QMouseEvent* event)
{
    if (m_editBySingleClick)
    {
        startInplaceEditor(nullptr);
        return true;
    }

    return ControllerMouse::processLButtonDown(event);
}

bool ControllerMouseInplaceEdit::processLButtonDblClick(QMouseEvent* /*event*/)
{
    startInplaceEditor(nullptr);
    return true;
}

void ControllerMouseInplaceEdit::stopCapturingImpl()
{
    ControllerMouse::stopCapturingImpl();

    if (m_inplaceEditor)
    {
        inplaceEdit().stop();
    }
}

void ControllerMouseInplaceEdit::doInplaceEditImpl(const QKeyEvent* keyEvent)
{
    startInplaceEditor(keyEvent);
}

void ControllerMouseInplaceEdit::updateInplaceEditLayoutImpl()
{
    // controller should be active
    Q_ASSERT(isCapturing());
    Q_ASSERT(m_inplaceEditor);
    Q_ASSERT(inplaceEdit().current() == m_inplaceEditor);

    if (!m_inplaceEditor)
        return;

    const ActivationState& aState = activationState();
    auto cacheItem = aState.cacheSpace.cacheItem(aState.visibleItem());
    if (!cacheItem)
    {
        // item become invisible -> stop editor
        inplaceEdit().stop();
        return;
    }

    m_inplaceEditor->setGeometry(editorRect());
}

void ControllerMouseInplaceEdit::stopInplaceEditor()
{
    if (!m_inplaceEditor)
    {
        // inconsistency
        Q_ASSERT(false);
    }

    Q_ASSERT(inplaceEdit().current() == m_inplaceEditor);

    disconnect(m_inplaceEditor, &QObject::destroyed, this, &ControllerMouseInplaceEdit::onInplaceEditorDestroyed);
    onInplaceEditorDestroyed(m_inplaceEditor);

    inplaceEdit().stop();
}

bool ControllerMouseInplaceEdit::startInplaceEditor(const QKeyEvent* keyEvent)
{
    if (m_inplaceEditor)
    {
        // inconsistency
        Q_ASSERT(false);
        return false;
    }

    const ActivationState& aState = activationState();
    m_inplaceEditor = createInplaceEditorImpl(aState.item, editorRect(), aState.context.widget, keyEvent);

    if (!m_inplaceEditor)
    {
        // normally it shouldn't happen
        Q_ASSERT(false);
        return false;
    }

    connect(m_inplaceEditor, &QObject::destroyed, this, &ControllerMouseInplaceEdit::onInplaceEditorDestroyed);

    emit inplaceEditStarted(aState.item, m_inplaceEditor);

    if (!m_inplaceEditor)
    {
        return false;
    }

    inplaceEdit().start(m_inplaceEditor);

    startCapturing();

    return true;
}

QRect ControllerMouseInplaceEdit::editorRect() const
{
    const ActivationState& aState = activationState();

    switch (m_layoutMode) {
    case InplaceEditLayoutModeView:
        return aState.viewRect;
    case InplaceEditLayoutModeItem:
        return aState.itemRect;
    default:
        return aState.viewRect;
    }
}

void ControllerMouseInplaceEdit::onInplaceEditorDestroyed(QObject* obj)
{
    Q_UNUSED(obj);
    Q_ASSERT(m_inplaceEditor == obj);
    m_inplaceEditor = nullptr;

    if (isCapturing())
        stopCapturing();

    emit inplaceEditFinished(activationState().item);
}

} // end namespace Qi
