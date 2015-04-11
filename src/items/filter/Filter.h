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

#ifndef QI_FILTER_H
#define QI_FILTER_H

#include "core/ItemID.h"
#include <QObject>

namespace Qi
{

class Model;

class QI_EXPORT ItemsFilter: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ItemsFilter)

public:
    virtual ~ItemsFilter();

    const QSharedPointer<Model>& modelToFilter() const { return m_modelToFilter; }
    bool isItemPassFilter(const ItemID& item) const { return isItemPassFilterImpl(item); }

signals:
    void filterChanged(const ItemsFilter*);

protected:
    ItemsFilter(const QSharedPointer<Model>& modelToFilter);

    virtual bool isItemPassFilterImpl(const ItemID& item) const = 0;

private:
    void onModelToFilterChanged(const Model*);

    QSharedPointer<Model> m_modelToFilter;
};

} // end namespace Qi

#endif // QI_FILTER_H
