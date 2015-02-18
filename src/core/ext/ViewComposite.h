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
    void cleanupDrawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache) const override;
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
