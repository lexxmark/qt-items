#include "SpaceWidgetAbstract.h"

namespace Qi
{

SpaceWidgetAbstract::SpaceWidgetAbstract(QWidget *parent)
    : QWidget(parent),
      SpaceWidgetCore(this)
{
}

SpaceWidgetAbstract::~SpaceWidgetAbstract()
{
}

bool SpaceWidgetAbstract::event(QEvent* event)
{
    bool result = QWidget::event(event);
    result |= processOwnerEvent(event);
    return result;
}

void SpaceWidgetAbstract::ensureVisibleImpl(const ItemID& /*visibleItem*/, const CacheSpace* /*cacheSpace*/, bool /*validateItem*/)
{
    // not applicable
}

} // end namespace Qi
