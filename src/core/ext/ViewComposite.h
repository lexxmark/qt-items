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

#ifndef QI_VIEW_COMPOSITE_H
#define QI_VIEW_COMPOSITE_H

#include "core/View.h"
#include "core/ItemSchema.h"
#include <QMargins>

namespace Qi
{

class QI_EXPORT ViewComposite: public View
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewComposite)

public:
    ViewComposite(const QVector<ViewSchema>& subViews, const QMargins& margins = QMargins());
    ViewComposite(const QSharedPointer<View>& subView, const QMargins& margins);

    virtual ~ViewComposite();

    const QMargins& margins() const { return m_margins; }
    void setMargins(const QMargins& margins);

protected:
    void addViewImpl(const ItemID& item, QVector<const View*>& views) const override;
    CacheView* addCacheViewImpl(const Layout& layout, const GuiContext& ctx, const ItemID& item, QVector<CacheView>& cacheViews, QRect& itemRect, QRect* visibleItemRect) const override;
    QSize sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const override;
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;
    //void cleanupDrawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache) const override;
    bool textImpl(const ItemID& item, QString& txt) const override;

private slots:
    void onSubViewChanged(const View* view, ChangeReason reason);

private:
    void connectSubViews();
    void disconnectSubViews();

    QVector<ViewSchema> m_subViews;
    // TODO: margins should be moved to Layout
    QMargins m_margins;
};

} // end namespace Qi

#endif // QI_VIEW_COMPOSITE_H
