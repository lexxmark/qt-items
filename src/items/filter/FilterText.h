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

#ifndef QI_FILTER_TEXT_H
#define QI_FILTER_TEXT_H

#include "Filter.h"
#include "space/Lines.h"
#include "items/text/Text.h"

namespace Qi
{

class View;

class QI_EXPORT ItemsFilterByText: public ItemsFilter
{
    Q_OBJECT
    Q_DISABLE_COPY(ItemsFilterByText)

public:
    const QString& filterText() const { return m_filterText; }
    bool setFilterText(const QString& filterText);

    bool isFilterTextEmpty() const { return m_filterText.isEmpty(); }

protected:
    ItemsFilterByText(const QSharedPointer<Model>& modelToFilter);

private:
    QString m_filterText;
};

class QI_EXPORT RowsFilterByText: public LinesVisibility
{
    Q_OBJECT
    Q_DISABLE_COPY(RowsFilterByText)

public:
    RowsFilterByText();
    virtual ~RowsFilterByText();

    QSharedPointer<ItemsFilterByText> filterByColumn(int column) const;
    bool addFilterByColumn(int column, const QSharedPointer<ItemsFilterByText>& filter);
    void clearFilters();

    bool isActive() const { return m_isActive; }
    void setActive(bool isActive);

protected:
    bool isLineVisibleImpl(int row) const override;

private:
    void onFilterChanged(const ItemsFilter*);

    mutable QVector<QSharedPointer<ItemsFilterByText>> m_filterByColumn;
    bool m_isActive;
};

QI_EXPORT QSharedPointer<View> makeViewRowsFilterByText(const QSharedPointer<RowsFilterByText>& filter);

class QI_EXPORT ItemsFilterTextByText: public ItemsFilterByText
{
    Q_OBJECT
    Q_DISABLE_COPY(ItemsFilterTextByText)

public:
    ItemsFilterTextByText(const QSharedPointer<ModelText>& modelText);

protected:
    bool isItemPassFilterImpl(const ItemID& item) const override;

private:
    QSharedPointer<ModelText> m_modelText;
};


} // end namespace Qi

#endif // QI_FILTER_TEXT_H
