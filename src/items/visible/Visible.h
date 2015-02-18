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

protected:
    void addViewImpl(const ItemID& item, QVector<const View*>& views) const override;
    CacheView* addCacheViewImpl(const Layout& layout, const GuiContext& ctx, const ItemID& item, QVector<CacheView>& cacheViews, QRect& itemRect, QRect* visibleItemRect) const override;
    QSize sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const override;

private:
    bool safeIsItemVisible(const ItemID& item) const;

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
