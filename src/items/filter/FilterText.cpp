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

#include "FilterText.h"

namespace Qi
{

ItemsFilterByText::ItemsFilterByText(SharedPtr<Model> modelToFilter)
    : ItemsFilter(std::move(modelToFilter))
{
}

bool ItemsFilterByText::setFilterText(const QString& filterText)
{
    if (m_filterText == filterText)
        return false;

    m_filterText = filterText;
    emit filterChanged(this);

    return true;
}

RowsFilterByText::RowsFilterByText()
    : m_isActive(true)
{
}

RowsFilterByText::~RowsFilterByText()
{
    clearFilters();
}

SharedPtr<ItemsFilterByText> RowsFilterByText::filterByColumn(int column) const
{
    if (m_filterByColumn.size() <= column)
        return nullptr;

    return m_filterByColumn[column];
}

bool RowsFilterByText::addFilterByColumn(int column, SharedPtr<ItemsFilterByText> filter)
{
    Q_ASSERT(!filter.isNull());

    if (m_filterByColumn.size() <= column)
        m_filterByColumn.resize(column + 1);

    if (!m_filterByColumn[column].isNull())
        return false;

    connect(filter.data(), &ItemsFilterByText::filterChanged, this, &RowsFilterByText::onFilterChanged);
    m_filterByColumn[column] = std::move(filter);

    return true;
}

void RowsFilterByText::clearFilters()
{
    for (const auto& filter: m_filterByColumn)
    {
        if (!filter.isNull())
            disconnect(filter.data(), &ItemsFilterByText::filterChanged, this, &RowsFilterByText::onFilterChanged);
    }
}

void RowsFilterByText::setActive(bool isActive)
{
    if (m_isActive == isActive)
        return;

    m_isActive = isActive;
    emit visibilityChanged(this);
}

bool RowsFilterByText::isLineVisibleImpl(int row) const
{
    for (GridID id(row, 0); id.column < m_filterByColumn.size(); ++id.column)
    {
        if (m_filterByColumn[id.column].isNull())
            continue;

        if (!m_filterByColumn[id.column]->isItemPassFilter(ID(id)))
            return false;
    }

    return true;
}

void RowsFilterByText::onFilterChanged(const ItemsFilter*)
{
    emit visibilityChanged(this);
}

SharedPtr<View2> makeViewRowsFilterByText(SharedPtr<RowsFilterByText> filter)
{
    auto modelFilterText = makeShared<ModelTextCallback>();
    modelFilterText->getValueFunction = [filter](ID id)->QString {
        auto subFilter = filter->filterByColumn(column(id));
        return subFilter.isNull() ? QString() : subFilter->filterText();
    };
    modelFilterText->setValueFunction = [filter](ID id, QString value)->bool {
        auto subFilter = filter->filterByColumn(column(id));
        if (subFilter.isNull())
            return false;

        return subFilter->setFilterText(value);
    };

    auto view = makeShared<ViewTextOrHint>(modelFilterText);
    view->isItemHint = [filter](ID id, const ModelText* sourceText) {
        if (filter->filterByColumn(column(id)).isNull()) return false;
        if (!sourceText) return false;
        return sourceText->value(id).isEmpty();
    };

    view->itemHintText = [](const ID&, const ModelText*)->QString {
        return "<filter>";
    };

    view->itemHintTooltipText = [](const ID&, const ModelText*, QString& text)->bool {
        text = "Enter text to filter";
        return true;
    };

    auto controller = makeShared<ControllerMouseText>(modelFilterText);
    controller->enableEditBySingleClick();
    controller->enableLiveUpdate();
    view->setController(controller);

    return view;
}

ItemsFilterTextByText::ItemsFilterTextByText(SharedPtr<ModelText> modelText)
    : ItemsFilterByText(modelText),
      m_modelText(std::move(modelText))
{
    Q_ASSERT(m_modelText);
}

bool ItemsFilterTextByText::isItemPassFilterImpl(ID id) const
{
    if (isFilterTextEmpty())
        return true;

    QString textValue = m_modelText->value(id);
    return textValue.contains(filterText());
}


} // end namespace Qi
