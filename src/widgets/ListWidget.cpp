#include "ListWidget.h"
#include "cache/space/CacheSpaceGrid.h"
#include "cache/CacheItem.h"

namespace Qi
{

ListWidget::ListWidget(QWidget* parent)
    : SpaceWidgetScrollAbstract(parent)
{
    // initialize main grid
    m_grid = QSharedPointer<SpaceGrid>::create();
    m_cacheGrid = QSharedPointer<CacheSpaceGrid>::create(m_grid);

    initSpaceWidgetCore(m_cacheGrid);
}

ListWidget::~ListWidget()
{
}


} // end namespace Qi
