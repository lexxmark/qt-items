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

#ifndef QI_VIEW_ITEM_VISIBLE_H
#define QI_VIEW_ITEM_VISIBLE_H

#include "core/View.h"
#include "core/ControllerMouse.h"

namespace Qi
{

class QI_EXPORT ViewVisible: public View
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewVisible)

public:
    explicit ViewVisible(const QSharedPointer<View>& sourceView, bool reserveSize = false);

    std::function<bool(const ItemID&)> isItemVisible;

    void notifyVisibilityChanged();

protected:
    void addViewImpl(const ItemID& item, QVector<const View*>& views) const override;
    CacheView* addCacheViewImpl(const Layout& layout, const GuiContext& ctx, const ItemID& item, QVector<CacheView>& cacheViews, QRect& itemRect, QRect* visibleItemRect) const override;
    QSize sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const override;

private:
    bool safeIsItemVisible(const ItemID& item) const;
    void onSourceViewChanged(const View* view, ChangeReason reason);

    QSharedPointer<View> m_sourceView;
    bool m_reserveSize;
};

class QI_EXPORT ControllerMouseVisible: public ControllerMouse
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerMouseVisible)

public:
    explicit ControllerMouseVisible(const QSharedPointer<ViewVisible>& view);

    bool event(QEvent* e) override;

protected:
    void activateImpl(const ActivationInfo& activationInfo) override;
    void deactivateImpl() override;

private:
    void notifyViewVisiblityChangedLater();

    QSharedPointer<ViewVisible> m_view;
};

} // end namespace Qi

#endif // QI_VIEW_ITEM_VISIBLE_H
