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
#include "space/SpaceGrid.h"

namespace Qi
{

ModelGridSortingBase::ModelGridSortingBase(const QSharedPointer<SpaceGrid>& grid)
    : m_grid(grid),
      m_ascending(false),
      m_sortingExpired(false)
{
}

void ModelGridSortingBase::clearActiveSortingItem()
{
    m_activeSortingItem = ItemID();
    emit modelChanged(this);
}

void ModelGridSortingBase::setSorting(const ItemID& item, bool ascending)
{
    if (!item.isValid())
        return;

    m_activeSortingItem = item;
    m_ascending = ascending;
    m_sortingExpired = true;
}

bool ModelGridSortingBase::sort()
{
    return sortByItem(m_activeSortingItem, m_ascending);
}

bool ModelGridSortingBase::sortByItem(const ItemID& item)
{
    if (m_activeSortingItem == item)
        return sortByItem(item, m_sortingExpired ? m_ascending : !m_ascending);
    else
        return defaultSortByItem(item);
}

bool ModelGridSortingBase::defaultSortByItem(const ItemID& item)
{
    auto model = sortingModel(item);
    if (!model)
        return false;

    if (!item.isValid())
        return false;

    m_activeSortingItem = item;
    m_ascending = model->isAscendingDefault(item);
    m_sortingExpired = false;

    emit willSortItems(this);

    m_grid->sortColumnByModel(item.column, model, m_ascending, true);

    emit didSortItems(this);
    emit modelChanged(this);

    return true;
}

bool ModelGridSortingBase::sortByItem(const ItemID& item, bool ascending)
{
    auto model = sortingModel(item);
    if (!model)
        return false;

    if (!item.isValid())
        return false;

    m_activeSortingItem = item;
    m_ascending = ascending;
    m_sortingExpired = false;

    emit willSortItems(this);

    m_grid->sortColumnByModel(item.column, model, m_ascending, true);

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
    if (!m_activeSortingItem.isValid())
        return;

    if (sortingModel(m_activeSortingItem).data() == model)
    {
        // mark sorting as expired
        m_sortingExpired = true;
    }
}

ModelGridSorting::ModelGridSorting(const QSharedPointer<SpaceGrid>& grid)
    : ModelGridSortingBase(grid)
{
}

ModelGridSorting::~ModelGridSorting()
{
    clear();
}

void ModelGridSorting::addSortingModel(const ItemID& item, const QSharedPointer<ModelComparable>& model)
{
    Q_ASSERT(model);

    if (!m_modelsToSort.contains(item))
    {
        m_modelsToSort.insert(item, model);
        connectModel(model.data());
    }
}

void ModelGridSorting::addSortingModel(int column, const QSharedPointer<ModelComparable>& model)
{
    addSortingModel(ItemID(0, column), model);
}

void ModelGridSorting::clear()
{
    for (auto it : m_modelsToSort)
    {
        disconnectModel(it.data());
    }

    m_modelsToSort.clear();
}

QSharedPointer<ModelComparable> ModelGridSorting::sortingModelImpl(const ItemID& item) const
{
    auto it = m_modelsToSort.find(item);
    if (it == m_modelsToSort.end())
        return QSharedPointer<ModelComparable>();

    return it.value();
}

ModelGridSortingByRanges::ModelGridSortingByRanges(const QSharedPointer<SpaceGrid> &grid)
    : ModelGridSortingBase(grid)
{
}

ModelGridSortingByRanges::~ModelGridSortingByRanges()
{
    clear();
}

void ModelGridSortingByRanges::addSortingModel(const QSharedPointer<ModelComparable>& model, const QSharedPointer<Range>& range)
{
    Q_ASSERT(model);

    SortingInfo info = { range, model };
    m_modelsToSort.append(info);
    connectModel(model.data());
}

void ModelGridSortingByRanges::clear()
{
    for (const auto& info : m_modelsToSort)
    {
        disconnectModel(info.model.data());
    }

    m_modelsToSort.clear();
}

QSharedPointer<ModelComparable> ModelGridSortingByRanges::sortingModelImpl(const ItemID& item) const
{
    for (const auto& info : m_modelsToSort)
    {
        if (info.range->hasItem(item))
            return info.model;
    }

    return QSharedPointer<ModelComparable>();
}

SortingHub::SortingHub()
{
}

SortingHub::SortingHub(const QSharedPointer<ModelGridSortingBase>& sorting1, const QSharedPointer<ModelGridSortingBase>& sorting2)
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

void SortingHub::addSorting(const QSharedPointer<ModelGridSortingBase>& sorting)
{
    Q_ASSERT(sorting);

    Info info;
    info.sorting = sorting;
    info.connection = QObject::connect(sorting.data(), &ModelGridSortingBase::didSortItems, [this] (const ModelGridSortingBase* activeSorting) {
        onDidSortItems(activeSorting);
    });
    m_sortings.append(info);
}

void SortingHub::clearActiveSortingItem()
{
    for (const auto& info : m_sortings)
    {
        info.sorting->clearActiveSortingItem();
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
            info.sorting->clearActiveSortingItem();
    }
}

RangeGridSorting::RangeGridSorting(const QSharedPointer<ModelGridSortingBase>& model, int row)
    : m_model(model),
      m_row(row)
{
    Q_ASSERT(m_model);
}

bool RangeGridSorting::hasItemImpl(const ItemID &item) const
{
    return m_row ==item.row && !m_model->sortingModel(item).isNull();
}

bool RangeGridSorting::hasRowImpl(int row) const
{
    return m_row == row;
}

bool RangeGridSorting::hasColumnImpl(int column) const
{
    return !m_model->sortingModel(ItemID(m_row, column)).isNull();
}

ViewGridSorting::ViewGridSorting(const QSharedPointer<ModelGridSortingBase>& model, bool useDefaultController)
    : ViewModeled<ModelGridSortingBase>(model)
{
    if (useDefaultController)
    {
        setController(QSharedPointer<ControllerMouseGridSorting>::create(model));
    }
}

void ViewGridSorting::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const
{
    QRect rect = cache.cacheView.rect();
    rect.adjust(4, 4, -4, -4);
    painter->drawRoundedRect(rect, 20.f, 20.f, Qt::RelativeSize);

    if (theModel()->activeSortingItem() == cache.item)
    {
        QStyleOptionHeader option;
        ctx.initStyleOption(option);
        option.sortIndicator = theModel()->isAscending() ? QStyleOptionHeader::SortUp : QStyleOptionHeader::SortDown;
        option.rect = rect;

        ctx.style()->drawPrimitive(QStyle::PE_IndicatorHeaderArrow, &option, painter, ctx.widget);
    }

    if (showTooltip) *showTooltip = true;
}

bool ViewGridSorting::tooltipTextImpl(const ItemID& item, QString& txt) const
{
    if (theModel()->activeSortingItem() == item)
    {
        txt = theModel()->isAscending() ? "Ascending" : "Descending";
    }
    else
    {
        txt = "Click to sort";
    }

    return true;
}

ControllerMouseGridSorting::ControllerMouseGridSorting(const QSharedPointer<ModelGridSortingBase>& model)
    : m_model(model)
{
}

void ControllerMouseGridSorting::applyImpl()
{
    m_model->sortByItem(activationState().item);
}

} // end namespace Qi
