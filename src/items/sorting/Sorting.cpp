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

#include "Sorting.h"
#include "space/grid/SpaceGrid.h"

namespace Qi
{

ModelGridSortingBase::ModelGridSortingBase(SharedPtr<SpaceGrid> grid)
    : m_grid(std::move(grid)),
      m_ascending(false),
      m_sortingExpired(false)
{
}

void ModelGridSortingBase::clearActiveSortingId()
{
    m_activeSortingId = GridID();
    emit modelChanged(this);
}

void ModelGridSortingBase::setSorting(GridID id, bool ascending)
{
    if (!id.isValid())
        return;

    m_activeSortingId = id;
    m_ascending = ascending;
    m_sortingExpired = true;
}

bool ModelGridSortingBase::sort()
{
    return sortByItem(m_activeSortingId, m_ascending);
}

bool ModelGridSortingBase::sortByItem(GridID id)
{
    if (m_activeSortingId == id)
        return sortByItem(id, m_sortingExpired ? m_ascending : !m_ascending);
    else
        return defaultSortByItem(id);
}

bool ModelGridSortingBase::defaultSortByItem(GridID id)
{
    auto model = sortingModel(id);
    if (!model)
        return false;

    if (!id.isValid())
        return false;

    m_activeSortingId = id;
    m_ascending = model->isAscendingDefault(ID(id));
    m_sortingExpired = false;

    emit willSortItems(this);

    m_grid->sortColumnByModel(id.column, *model, m_ascending, true);

    emit didSortItems(this);
    emit modelChanged(this);

    return true;
}

bool ModelGridSortingBase::sortByItem(GridID id, bool ascending)
{
    auto model = sortingModel(id);
    if (!model)
        return false;

    if (!id.isValid())
        return false;

    m_activeSortingId = id;
    m_ascending = ascending;
    m_sortingExpired = false;

    emit willSortItems(this);

    m_grid->sortColumnByModel(id.column, *model, m_ascending, true);

    emit didSortItems(this);
    emit modelChanged(this);

    return true;
}

void ModelGridSortingBase::connectModel(const Model* model)
{
    connect(model, &Model::modelChanged, this, &ModelGridSortingBase::onSortingModelChanged);
}

void ModelGridSortingBase::disconnectModel(const Model* model)
{
    disconnect(model, &Model::modelChanged, this, &ModelGridSortingBase::onSortingModelChanged);
}

void ModelGridSortingBase::onSortingModelChanged(const Model* model)
{
    if (!m_activeSortingId.isValid())
        return;

    if (sortingModel(m_activeSortingId).data() == model)
    {
        // mark sorting as expired
        m_sortingExpired = true;
    }
}

ModelGridSorting::ModelGridSorting(SharedPtr<SpaceGrid> grid)
    : ModelGridSortingBase(std::move(grid))
{
}

ModelGridSorting::~ModelGridSorting()
{
    clear();
}

void ModelGridSorting::addSortingModel(GridID id, SharedPtr<ModelComparable> model)
{
    Q_ASSERT(model);

    if (!m_modelsToSort.contains(id))
    {
        connectModel(model.data());
        m_modelsToSort.insert(id, std::move(model));
    }
}

void ModelGridSorting::addSortingModel(int column, SharedPtr<ModelComparable> model)
{
    addSortingModel(GridID(0, column), std::move(model));
}

void ModelGridSorting::clear()
{
    for (auto it : m_modelsToSort)
    {
        disconnectModel(it.data());
    }

    m_modelsToSort.clear();
}

SharedPtr<ModelComparable> ModelGridSorting::sortingModelImpl(GridID id) const
{
    auto it = m_modelsToSort.find(id);
    if (it == m_modelsToSort.end())
        return nullptr;

    return it.value();
}

ModelGridSortingByRanges::ModelGridSortingByRanges(SharedPtr<SpaceGrid> grid)
    : ModelGridSortingBase(std::move(grid))
{
}

ModelGridSortingByRanges::~ModelGridSortingByRanges()
{
    clear();
}

void ModelGridSortingByRanges::addSortingModel(SharedPtr<ModelComparable> model, SharedPtr<Range> range)
{
    Q_ASSERT(model);

    connectModel(model.data());
    SortingInfo info = { std::move(range), std::move(model) };
    m_modelsToSort.append(std::move(info));
}

void ModelGridSortingByRanges::clear()
{
    for (const auto& info : m_modelsToSort)
    {
        disconnectModel(info.model.data());
    }

    m_modelsToSort.clear();
}

SharedPtr<ModelComparable> ModelGridSortingByRanges::sortingModelImpl(GridID id) const
{
    for (const auto& info : m_modelsToSort)
    {
        if (info.range->hasItem(ID(id)))
            return info.model;
    }

    return nullptr;
}

SortingHub::SortingHub()
{
}

SortingHub::SortingHub(const SharedPtr<ModelGridSortingBase> &sorting1, const SharedPtr<ModelGridSortingBase> &sorting2)
{
    addSorting(sorting1);
    addSorting(sorting2);
}

SortingHub::~SortingHub()
{
    for (const auto& info : m_sortings)
    {
        QObject::disconnect(info.connection);
    }
}

void SortingHub::addSorting(SharedPtr<ModelGridSortingBase> sorting)
{
    Q_ASSERT(sorting);

    Info info;
    info.sorting = std::move(sorting);
    info.connection = QObject::connect(sorting.data(), &ModelGridSortingBase::didSortItems, [this] (const ModelGridSortingBase* activeSorting) {
        onDidSortItems(activeSorting);
    });
    m_sortings.append(std::move(info));
}

void SortingHub::clearActiveSortingId()
{
    for (const auto& info : m_sortings)
    {
        info.sorting->clearActiveSortingId();
    }
}

void SortingHub::sort()
{
    for (const auto& info : m_sortings)
    {
        info.sorting->sort();
    }
}

void SortingHub::onDidSortItems(const ModelGridSortingBase* activeSorting)
{
    for (const auto& info : m_sortings)
    {
        if (info.sorting.data() != activeSorting)
            info.sorting->clearActiveSortingId();
    }
}

RangeGridSorting::RangeGridSorting(SharedPtr<ModelGridSortingBase> model, int row)
    : m_model(std::move(model)),
      m_row(row)
{
    Q_ASSERT(m_model);
}

bool RangeGridSorting::hasItemImpl(ID id) const
{
    return m_row == row(id) && !m_model->sortingModel(id.as<GridID>()).isNull();
}

ViewGridSorting::ViewGridSorting(SharedPtr<ModelGridSortingBase> model, bool useDefaultController)
    : ViewModeled<ModelGridSortingBase>(std::move(model))
{
    if (useDefaultController)
    {
        setController(makeShared<ControllerMouseGridSorting>(theModel()));
    }
}

void ViewGridSorting::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const
{
    QRect rect = cache.cacheView.rect();
    rect.adjust(4, 4, -4, -4);
    painter->drawRoundedRect(rect, 20.f, 20.f, Qt::RelativeSize);

    if (theModel()->activeSortingId() == cache.id.as<GridID>())
    {
        QStyleOptionHeader option;
        ctx.initStyleOption(option);
        option.sortIndicator = theModel()->isAscending() ? QStyleOptionHeader::SortUp : QStyleOptionHeader::SortDown;
        option.rect = rect;

        ctx.style()->drawPrimitive(QStyle::PE_IndicatorHeaderArrow, &option, painter, ctx.widget);
    }

    if (showTooltip) *showTooltip = true;
}

bool ViewGridSorting::tooltipTextImpl(ID id, QString& txt) const
{
    if (theModel()->activeSortingId() == id.as<GridID>())
    {
        txt = theModel()->isAscending() ? "Ascending" : "Descending";
    }
    else
    {
        txt = "Click to sort";
    }

    return true;
}

ControllerMouseGridSorting::ControllerMouseGridSorting(SharedPtr<ModelGridSortingBase> model)
    : m_model(std::move(model))
{
}

void ControllerMouseGridSorting::applyImpl()
{
    m_model->sortByItem(activationState().id.as<GridID>());
}

} // end namespace Qi
