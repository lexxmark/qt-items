#ifndef QI_PAINTER_STATE_H
#define QI_PAINTER_STATE_H

#include "QiAPI.h"

class QPainter;
class QWidget;

namespace Qi
{

QI_EXPORT void copyPainterState(const QPainter* painterSource, QPainter* painterDest);
QI_EXPORT void copyPainterState(const QWidget* widgetSource, QPainter* painterDest);

} // end namespace Qi

#endif // QI_PAINTER_STATE_H
