#include "ControllerMouseAuxiliary.h"

namespace Qi
{

ControllerContext::ControllerContext(QWidget* widget, WidgetDriver* widgetDriver)
    : widget(widget),
      widgetDriver(widgetDriver),
      point(0, 0)
{
}

ControllerContext::~ControllerContext()
{
}

void ControllerContext::pushCursor(QCursor cursor, const ControllerMouse* controller) const
{
    Q_ASSERT(std::find_if(m_cursorInfos.begin(), m_cursorInfos.end(), [controller](const QPair<QCursor, const ControllerMouse*>& elem) {
        return elem.second == controller;
    }) == m_cursorInfos.end());

    if (m_cursorInfos.isEmpty())
        m_widgetCursor = widget->cursor();

    m_cursorInfos.append(qMakePair(cursor, controller));
    widget->setCursor(cursor);
}

void ControllerContext::popCursor(const ControllerMouse* controller) const
{
    for (auto it = m_cursorInfos.begin(); it != m_cursorInfos.end(); ++it)
    {
        if (it->second == controller)
        {
            m_cursorInfos.erase(it);
            break;
        }
    }

    if (m_cursorInfos.isEmpty())
        widget->setCursor(m_widgetCursor);
    else
        widget->setCursor(m_cursorInfos.back().first);
}

} // end namespace Qi
