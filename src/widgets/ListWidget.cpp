/*
   Copyright (c) 2008-1015 Alex Zhondin <qtinuum.team@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "ListWidget.h"
#include "cache/space/CacheSpaceGrid.h"
#include "cache/space/CacheSpaceItem.h"
#include "cache/CacheItemFactory.h"
#include "items/cache/ViewCacheSpace.h"
#include "items/visible/Visible.h"
#include "core/ext/Ranges.h"
#include "core/ext/Layouts.h"
#include "utils/PainterState.h"

namespace Qi
{

ListWidget::ListWidget(QWidget* parent)
    : SpaceWidgetScrollAbstract(parent)
{
    // initialize main grid
    m_grid = QSharedPointer<SpaceGrid>::create();
    m_cacheGrid = QSharedPointer<CacheSpaceGrid>::create(m_grid);

    connect(m_cacheGrid.data(), &CacheSpace::cacheChanged, this, &ListWidget::onCacheSpaceGridChanged);

    // initialize main item space
    m_mainSpace = QSharedPointer<SpaceItem>::create(ItemID(0, 0));
    m_mainCache = QSharedPointer<CacheSpaceItem>::create(m_mainSpace, true);

    // add grid cache to main schema
    auto modelCache = QSharedPointer<ModelStorageValue<QSharedPointer<CacheSpace>>>::create(m_cacheGrid);
    m_mainSpace->addSchema(makeRangeAll(), QSharedPointer<ViewCacheSpace>::create(modelCache), makeLayoutBackground());

    initSpaceWidgetScrollable(m_mainCache, m_cacheGrid);
}

ListWidget::~ListWidget()
{
}

void ListWidget::onCacheSpaceGridChanged(const CacheSpace* cache, ChangeReason /*reason*/)
{
    Q_UNUSED(cache);
    Q_ASSERT(cache == m_cacheGrid.data());
    viewport()->update();;
}

void ListWidget::onSpaceGridChanged(const Space* space, ChangeReason reason)
{
    Q_UNUSED(space);
    Q_ASSERT(space == m_grid.data());
    Q_ASSERT(!m_emptyView.isNull());

    if (reason & ChangeReasonSpaceStructure)
        m_emptyView->notifyVisibilityChanged();
}

bool ListWidget::installEmptyView(const QSharedPointer<View>& view, const QSharedPointer<Layout>& layout)
{
    Q_ASSERT(view);

    if (m_emptyView)
        return false;

    m_emptyView = QSharedPointer<ViewVisible>::create(view);
    m_emptyView->isItemVisible = [this](const ItemID&)->bool {
        return m_grid->isEmptyVisible();
    };
    connect(m_grid.data(), &Space::spaceChanged, this, &ListWidget::onSpaceGridChanged);

    m_mainSpace->addSchema(makeRangeAll(), m_emptyView, layout);

    return true;
}

QPixmap ListWidget::createPixmapImpl() const
{
    if (m_grid->isEmptyVisible())
        return SpaceWidgetScrollAbstract::createPixmapImpl();

    QPixmap image(m_grid->size());
    image.fill(viewport()->palette().color(viewport()->backgroundRole()));

    {
        QPainter painter(&image);
        copyPainterState(viewport(), &painter);

        auto cacheItemFactory = m_grid->createCacheItemFactory(ViewApplicationCopyDraw);
        for (ItemsIteratorGridVisible it(*m_grid); it.isValid(); it.toNext())
        {
            CacheItem cacheItem(cacheItemFactory->create(it.itemVisible()));
            cacheItem.drawRaw(&painter, guiContext());
        }
    }
    return image;
}


} // end namespace Qi
