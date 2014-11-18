#ifndef QI_VIEWS_H
#define QI_VIEWS_H

#include "core/View.h"
#include "core/Model.h"

namespace Qi
{

class QI_EXPORT ViewCallback: public View
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewCallback)

public:
    ViewCallback()
    {}

    std::function<void(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip)> drawFunction;
    std::function<QSize(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode)> sizeFunction;
    std::function<bool(const ItemID& item, QString& txt)> textFunction;


protected:
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;
    QSize sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const override;
    bool textImpl(const ItemID& item, QString& txt) const override;
};

class QI_EXPORT ViewVisible: public View
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewVisible)

public:
    explicit ViewVisible(const QSharedPointer<View>& sourceView);
    ViewVisible(const QSharedPointer<View>& sourceView, const std::function<bool(const ItemID&)>& isVisibleFunction);

    virtual ~ViewVisible();

    std::function<bool(const ItemID&)> isVisibleFunction;

    bool isVisible(const ItemID& item) const { return isVisibleFunction ? isVisibleFunction(item) : false; }
    const QSharedPointer<View>& sourceView() const { return m_sourceView; }

protected:
    void addViewImpl(const ItemID& item, QVector<const View*>& views) const override;
    CacheView* addCacheViewImpl(const Layout& layout, const GuiContext& ctx, const ItemID& item, QVector<CacheView>& cacheViews, QRect& itemRect, QRect* visibleItemRect) const override;

private slots:
    void onSourceViewChanged(const View* view, ChangeReason reason);

private:
    QSharedPointer<View> m_sourceView;
};

class QI_EXPORT ViewVisibleWithSize: public ViewVisible
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewVisibleWithSize)

public:
    explicit ViewVisibleWithSize(const QSharedPointer<View>& sourceView);
    ViewVisibleWithSize(const QSharedPointer<View>& sourceView, const std::function<bool(const ItemID&)>& isVisibleFunction);

protected:
    QSize sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const override;
};

} // end namespace Qi

#endif // QI_VIEWS_H
