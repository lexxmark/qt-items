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

ItemsFilterByText::ItemsFilterByText(const QSharedPointer<Model>& modelToFilter)
    : ItemsFilter(modelToFilter)
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

QSharedPointer<ItemsFilterByText> RowsFilterByText::filterByColumn(int column) const
{
    if (m_filterByColumn.size() <= column)
        return QSharedPointer<ItemsFilterByText>();

    return m_filterByColumn[column];
}

bool RowsFilterByText::addFilterByColumn(int column, const QSharedPointer<ItemsFilterByText>& filter)
{
    Q_ASSERT(!filter.isNull());

    if (m_filterByColumn.size() <= column)
        m_filterByColumn.resize(column + 1);

    if (!m_filterByColumn[column].isNull())
        return false;

    m_filterByColumn[column] = filter;
    connect(filter.data(), &ItemsFilterByText::filterChanged, this, &RowsFilterByText::onFilterChanged);
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
    for (ItemID item(row, 0); item.column < m_filterByColumn.size(); ++item.column)
    {
        if (m_filterByColumn[item.column].isNull())
            continue;

        if (!m_filterByColumn[item.column]->isItemPassFilter(item))
            return false;
    }

    return true;
}

void RowsFilterByText::onFilterChanged(const ItemsFilter*)
{
    emit visibilityChanged(this);
}

QSharedPointer<View> makeViewRowsFilterByText(const QSharedPointer<RowsFilterByText>& filter)
{
    auto modelFilterText = QSharedPointer<ModelTextCallback>::create();
    modelFilterText->getValueFunction = [filter](const ItemID& item)->QString {
        auto subFilter = filter->filterByColumn(item.column);
        return subFilter.isNull() ? QString() : subFilter->filterText();
    };
    modelFilterText->setValueFunction = [filter](const ItemID& item, QString value)->bool {
        auto subFilter = filter->filterByColumn(item.column);
        if (subFilter.isNull())
            return false;

        return subFilter->setFilterText(value);
    };

    auto view = QSharedPointer<ViewTextOrHint>::create(modelFilterText);
    view->isItemHint = [filter](const ItemID& item, const ModelText* sourceText) {
        if (filter->filterByColumn(item.column).isNull()) return false;
        if (!sourceText) return false;
        return sourceText->value(item).isEmpty();
    };

    view->itemHintText = [](const ItemID&, const ModelText*)->QString {
        return "<filter>";
    };

    view->itemHintTooltipText = [](const ItemID&, const ModelText*, QString& text)->bool {
        text = "Enter text to filter";
        return true;
    };

    auto controller = QSharedPointer<ControllerMouseText>::create(modelFilterText);
    controller->enableEditBySingleClick();
    controller->enableLiveUpdate();
    view->setController(controller);

    return view;
}

ItemsFilterTextByText::ItemsFilterTextByText(const QSharedPointer<ModelText>& modelText)
    : ItemsFilterByText(modelText),
      m_modelText(modelText)
{
    Q_ASSERT(modelText);
}

bool ItemsFilterTextByText::isItemPassFilterImpl(const ItemID& item) const
{
    if (isFilterTextEmpty())
        return true;

    QString textValue = m_modelText->value(item);
    return textValue.contains(filterText());
}


} // end namespace Qi
