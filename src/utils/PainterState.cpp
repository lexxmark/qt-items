#include "PainterState.h"
#include <QPainter>
#include <QWidget>

namespace Qi
{

void copyPainterState(const QPainter* painterSource, QPainter* painterDest)
{
    Q_ASSERT(painterSource && painterDest);

    painterDest->setBackground(painterSource->background());
    painterDest->setBrush(painterSource->brush());
    painterDest->setCompositionMode(painterSource->compositionMode());
    painterDest->setFont(painterSource->font());
    painterDest->setLayoutDirection(painterSource->layoutDirection());
    painterDest->setOpacity(painterSource->opacity());
    painterDest->setPen(painterSource->pen());
    painterDest->setRenderHints(painterSource->renderHints());
}

void copyPainterState(const QWidget* widgetSource, QPainter* painterDest)
{
    Q_ASSERT(widgetSource && painterDest);

    QPalette palette = widgetSource->palette();
    painterDest->setBackground(palette.color(widgetSource->backgroundRole()));
    painterDest->setPen(palette.color(widgetSource->foregroundRole()));
    painterDest->setFont(widgetSource->font());
}

} // end namespace Qi
