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
