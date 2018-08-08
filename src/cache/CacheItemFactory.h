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

#ifndef QI_CACHE_ITEM_FACTORY_H
#define QI_CACHE_ITEM_FACTORY_H

#include "space/Space.h"
#include "CacheItem.h"

namespace Qi
{

class QI_EXPORT CacheItemFactory
{
    Q_DISABLE_COPY(CacheItemFactory)

public:
    CacheItemFactory(const Space2& space);
    virtual ~CacheItemFactory();

    CacheItemInfo create(ID visibleId) const;
    void updateSchema(CacheItemInfo& info) const;

    const Space2& space() const { return m_space; }

protected:
    virtual void initSchemaImpl(CacheItemInfo& info) const;

    ViewSchema createViewSchema(ID absId) const;

private:
    const Space2& m_space;
};

QI_EXPORT SharedPtr<CacheItemFactory> createCacheItemFactoryDefault(const Space2& space);
QI_EXPORT SharedPtr<CacheItemFactory> createCacheItemFactoryItem(const Space2& space);
QI_EXPORT SharedPtr<CacheItemFactory> createCacheItemFactorySameSchemaByColumn(const Space2& space);
QI_EXPORT SharedPtr<CacheItemFactory> createCacheItemFactorySameSchemaByRow(const Space2& space);

} // end namespace Qi

#endif // QI_CACHE_ITEM_FACTORY_H
