#ifndef QI_CHECK_H
#define QI_CHECK_H

#include "core/ext/ViewModeled.h"
#include "core/ext/ControllerMousePushable.h"
#include "core/ext/ModelCallback.h"

namespace Qi
{

typedef ModelTyped<Qt::CheckState> ModelCheck;
typedef ModelCallback<Qt::CheckState, false> ModelCheckCallback;

class QI_EXPORT ViewCheck: public ViewModeled<ModelCheck>
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewCheck)

public:
    ViewCheck(const QSharedPointer<ModelCheck>& model, bool useDefaultController = true);

protected:
    QSize sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const override;
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;

private:
    QStyle::State styleState(const ItemID& item) const;

    PushableTracker m_pushableTracker;
};

QI_EXPORT QSharedPointer<ControllerMousePushable> createControllerMouseCheck(const QSharedPointer<ModelCheck>& model);

} // end namespace Qi

#endif // QI_CHECK_H
