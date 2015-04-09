#ifndef QI_BUTTON_H
#define QI_BUTTON_H

#include "core/ext/ViewComposite.h"
#include "core/ext/ControllerMousePushable.h"

namespace Qi
{

class QI_EXPORT ViewButton: public ViewComposite
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewButton)

public:
    ViewButton(const QSharedPointer<View>& bttnContent, ViewDefaultController createDefaultController = ViewDefaultControllerCreate);

    std::function<void (const ItemID& item, QStyle::State& bttnState)> tuneBttnState;
    std::function<void (const ItemID& item, const ControllerContext& context, const ViewButton* viewBttn)> action;

protected:
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;

private:
    PushableTracker m_pushableTracker;
};

} // end namespace Qi

#endif // QI_BUTTON_H
