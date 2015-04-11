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

#ifndef QI_VIEW_ITEM_BORDER_H
#define QI_VIEW_ITEM_BORDER_H

#include "core/View.h"

namespace Qi
{

class QI_EXPORT ViewRowBorder: public View
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewRowBorder)

public:
    ViewRowBorder();

protected:
    QSize sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const override;
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;

private:
    mutable QColor m_gridColor;
};

class QI_EXPORT ViewColumnBorder: public View
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewColumnBorder)

public:
    ViewColumnBorder();

protected:
    QSize sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const override;
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;

private:
    mutable QColor m_gridColor;
};

class QI_EXPORT ViewRectBorder: public View
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewRectBorder)

public:
    ViewRectBorder();

protected:
    QSize sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const override;
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;

private:
    mutable QColor m_gridColor;
};

} // end namespace Qi

#endif // QI_VIEW_ITEM_BORDER_H
