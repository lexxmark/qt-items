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
    ViewButton(const QSharedPointer<View>& bttnContent, bool useDefaultController = true, quintptr id = 0);

    quintptr id() const { return m_id; }

    //Delegate_t<UINT(const CellID&)> getStateFunc;
    std::function<void (const ItemID& item, const ControllerContext& context, const ViewButton* viewBttn)> bttnAction;

protected:
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;
    void cleanupDrawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache) const override;

private:
    quintptr m_id;
    PushableTracker m_pushableTracker;
    mutable PainterState m_painterState;
};

} // end namespace Qi

#endif // QI_BUTTON_H
