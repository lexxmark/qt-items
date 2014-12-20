#include "Color.h"
#include "items/misc/ControllerMousePushableCallback.h"
#include <QColorDialog>

namespace Qi
{

ViewColor::ViewColor(const QSharedPointer<ModelColor>& model, bool useDefaultController, bool withBorder)
    : ViewModeled<ModelColor>(model),
      m_withBorder(withBorder)
{
    if (useDefaultController)
    {
        setController(createControllerMouseColor(model));
    }
}

void ViewColor::drawImpl(QPainter* painter, const GuiContext& /*ctx*/, const CacheContext& cache, bool* /*showTooltip*/) const
{
    QColor color = theModel()->value(cache.item);
    if (!color.isValid())
        return;

    QRect rect = cache.cacheView.rect();

    if (m_withBorder)
    {
        rect.adjust(2, 2, -2, -2);

        QBrush oldBrush = painter->brush();
        painter->setBrush(color);
        painter->drawRect(rect);
        painter->setBrush(oldBrush);
    }
    else
    {
        painter->fillRect(rect, color);
    }
}

QSharedPointer<ControllerMousePushable> createControllerMouseColor(const QSharedPointer<ModelColor>& model)
{
    auto controller = QSharedPointer<ControllerMousePushableCallback>::create();
    controller->onApply = [model] (const ItemID& item, const ControllerContext& context) {
        Q_ASSERT(item.isValid());
        QColorDialog dlg(model->value(item), context.widget);
        if (dlg.exec() == QDialog::Accepted)
        {
            model->setValue(item, dlg.currentColor());
        }
    };
    return controller;
}

} // end namespace Qi
