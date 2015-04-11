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

#ifndef QI_PIXMAP_H
#define QI_PIXMAP_H

#include "core/ext/ViewModeled.h"
#include "core/ext/ModelCallback.h"
#include "core/ext/ModelStore.h"
#include <QPixmap>

namespace Qi
{

namespace Private
{
    template <>
    inline int compareValues(const QPixmap& /*leftValue*/, const QPixmap& /*rightValue*/)
    {
        return 0;
    }

    class NotEqPixmaps: std::binary_function<QPixmap, QPixmap, bool>
    {
    public:
        bool operator()(const QPixmap&, const QPixmap&) const
        { return false; }
    };
}

typedef ModelTyped<QPixmap> ModelPixmap;
typedef ModelCallback<QPixmap> ModelPixmapCallback;
typedef ModelStorageValue<QPixmap, QPixmap, Private::NotEqPixmaps> ModelPixmapValue;

class QI_EXPORT ViewPixmap: public ViewModeled<ModelPixmap>
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewPixmap)

public:
    ViewPixmap(const QSharedPointer<ModelPixmap>& model);

protected:
    QSize sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const override;
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;
};

} // end namespace Qi

#endif // QI_PIXMAP_H
