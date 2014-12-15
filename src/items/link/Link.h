#ifndef QI_LINK_H
#define QI_LINK_H

#include "items/text/Text.h"
#include "items/misc/ControllerMousePushableCallback.h"

namespace Qi
{

class QI_EXPORT ViewLink: public ViewText
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewLink)

public:
    ViewLink(const QSharedPointer<ModelText>& model, bool useDefaultController = true, Qt::Alignment alignment = Qt::Alignment(Qt::AlignLeft | Qt::AlignVCenter), Qt::TextElideMode textElideMode = Qt::ElideNone);

    std::function<void (const ItemID& item, const ControllerContext& context, const ViewLink* viewLink)> action;

protected:
    //QSize sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const override;
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;

private:
    PushableTracker m_pushableTracker;
};

class ControllerMouseLink: public ControllerMousePushableCallback
{
    Q_OBJECT
    Q_DISABLE_COPY(ControllerMouseLink)

public:
    ControllerMouseLink(ControllerMousePriority priority = ControllerMousePriorityNormal, bool processDblClick = true);

protected:
    void activateImpl(const ActivationInfo& activationInfo) override;
    void deactivateImpl() override;

private:
    QCursor m_cursor;
};

} // end namespace Qi

#endif // QI_LINK_H
