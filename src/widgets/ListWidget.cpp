#include "ListWidget.h"
#include "cache/space/CacheSpaceGrid.h"
#include "cache/CacheItem.h"
//#include "items/cache/ViewCacheSpace.h"
//#include "core/ext/Ranges.h"
//#include "core/ext/Layouts.h"
#include <QScrollBar>

namespace Qi
{

ListWidget::ListWidget(QWidget* parent)
    : SpaceWidgetScrollAbstract(parent)
{
    // initialize main grid
    m_grid = QSharedPointer<SpaceGrid>::create();
    m_cacheGrid = QSharedPointer<CacheSpaceGrid>::create(m_grid);

    // add sub-grid caches to main grid schema
//    m_mainGrid->addSchema(makeRangeAll(), QSharedPointer<ViewCacheSpace>::create(modelCache), makeLayoutBackground());

    initSpaceWidgetCore(m_cacheGrid);
}

ListWidget::~ListWidget()
{
}


} // end namespace Qi
