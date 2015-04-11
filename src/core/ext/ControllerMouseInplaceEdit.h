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

#ifndef QI_CONTROLLER_MOUSE_INPLACE_EDIT_H
#define QI_CONTROLLER_MOUSE_INPLACE_EDIT_H

#include "core/ControllerMouse.h"

class QWidget;

namespace Qi
{

enum InplaceEditLayoutMode
{
    InplaceEditLayoutModeView,
    InplaceEditLayoutModeItem
};

class QI_EXPORT ControllerMouseInplaceEdit: public ControllerMouse
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerMouseInplaceEdit)

public:
    void enableEditBySingleClick(bool enable = true) { m_editBySingleClick = enable; }
    void setEditLayoutMode(InplaceEditLayoutMode layoutMode) { m_layoutMode = layoutMode; }

signals:
    void inplaceEditStarted(const ItemID& item, QWidget* editor);
    void inplaceEditFinished(const ItemID& item);

protected:
    ControllerMouseInplaceEdit(ControllerMousePriority priority = ControllerMousePriorityNormal);

    virtual QWidget* createInplaceEditorImpl(const ItemID& item, const QRect& rect, QWidget* parent, const QKeyEvent* keyEvent) = 0;

    bool processLButtonDown(QMouseEvent* event) override;
    bool processLButtonDblClick(QMouseEvent* event) override;
    void stopCapturingImpl() override;
    void doInplaceEditImpl(const QKeyEvent* keyEvent) override;
    void updateInplaceEditLayoutImpl() override;

    template<typename T>
    T* inplaceEditor() { return qobject_cast<T*>(m_inplaceEditor); }

    void stopInplaceEditor();

private:
    bool startInplaceEditor(const QKeyEvent* keyEvent);
    QRect editorRect() const;
    void onInplaceEditorDestroyed(QObject* obj);

    bool m_editBySingleClick;
    InplaceEditLayoutMode m_layoutMode;
    QWidget* m_inplaceEditor;
};

} // end namespace Qi

#endif // QI_CONTROLLER_MOUSE_INPLACE_EDIT_H
