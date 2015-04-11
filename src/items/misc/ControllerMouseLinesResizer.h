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

#ifndef QI_CONTROLLER_MOUSE_LINES_RESIZER_H
#define QI_CONTROLLER_MOUSE_LINES_RESIZER_H

#include "core/ext/ControllerMouseCaptured.h"
#include <QCursor>

class QRubberBand;

namespace Qi
{

class Lines;

class QI_EXPORT ControllerMouseColumnsResizer: public ControllerMouseCaptured
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerMouseColumnsResizer)

public:
    ControllerMouseColumnsResizer(const QSharedPointer<Lines>& columns, ControllerMousePriority priority = ControllerMousePriorityNormal);

    bool processMouseMove(QMouseEvent* event) override;

protected:
    bool acceptImpl(const ActivationInfo& activationInfo) const override;
    void activateImpl(const ActivationInfo& activationInfo) override;
    void deactivateImpl() override;

    void startCapturingImpl() override;
    void stopCapturingImpl() override;

    void applyImpl() override;
    bool canApplyImpl() const override { return true; }

private:
    QSharedPointer<Lines> m_columns;
    mutable int m_delta;
    int m_position;
    int m_trackPosition;
    int m_columnIndex;
    QRubberBand* m_rubberBand;
};

class QI_EXPORT ControllerMouseRowsResizer: public ControllerMouseCaptured
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerMouseRowsResizer)

public:
    ControllerMouseRowsResizer(const QSharedPointer<Lines>& rows, ControllerMousePriority priority = ControllerMousePriorityNormal);

    bool processMouseMove(QMouseEvent* event) override;

protected:
    bool acceptImpl(const ActivationInfo& activationInfo) const override;
    void activateImpl(const ActivationInfo& activationInfo) override;
    void deactivateImpl() override;

    void startCapturingImpl() override;
    void stopCapturingImpl() override;

    void applyImpl() override;
    bool canApplyImpl() const override { return true; }

private:
    QSharedPointer<Lines> m_rows;
    mutable int m_delta;
    int m_position;
    int m_trackPosition;
    int m_rowIndex;
    QRubberBand* m_rubberBand;
};

} // end namespace Qi

#endif // QI_CONTROLLER_MOUSE_LINES_RESIZER_H
