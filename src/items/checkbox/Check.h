#ifndef QI_CHECK_H
#define QI_CHECK_H

#include "core/ext/ViewModeled.h"
#include "core/ext/ControllerMousePushable.h"
#include "core/ext/ModelCallback.h"
#include "core/ext/ModelStore.h"

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

class QI_EXPORT ControllerMouseCheck: public ControllerMousePushable
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerMouseCheck)

public:
    ControllerMouseCheck(const QSharedPointer<ModelCheck>& model);

protected:
    void applyImpl() override;
    bool acceptEditImpl(const ItemID& item, const CacheSpace& cacheSpace, const QKeyEvent* keyEvent) const override;
    void doEditImpl(const QKeyEvent* keyEvent) override;

private:
    void toggleCheck();

    QSharedPointer<ModelCheck> m_model;
};

} // end namespace Qi

#endif // QI_CHECK_H
