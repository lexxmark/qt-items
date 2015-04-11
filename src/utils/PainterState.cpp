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
