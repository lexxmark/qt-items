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
    CacheItemFactory(const Space& space, ViewApplicationMask viewApplicationMask);
    virtual ~CacheItemFactory();

    CacheItemInfo create(const ItemID& visibleItem) const;
    void updateSchema(CacheItemInfo& info) const;

    const Space& space() const { return m_space; }

protected:
    virtual void initSchemaImpl(CacheItemInfo& info) const;

    ViewSchema createViewSchema(const ItemID& absItem) const;

private:
    const Space& m_space;
    ViewApplicationMask m_viewApplicationMask;
};

QI_EXPORT QSharedPointer<CacheItemFactory> createCacheItemFactoryDefault(const Space& space, ViewApplicationMask viewApplicationMask);
QI_EXPORT QSharedPointer<CacheItemFactory> createCacheItemFactoryItem(const Space& space, ViewApplicationMask viewApplicationMask);
QI_EXPORT QSharedPointer<CacheItemFactory> createCacheItemFactorySameSchemaByColumn(const Space& space, ViewApplicationMask viewApplicationMask);
QI_EXPORT QSharedPointer<CacheItemFactory> createCacheItemFactorySameSchemaByRow(const Space& space, ViewApplicationMask viewApplicationMask);

} // end namespace Qi

#endif // QI_CACHE_ITEM_FACTORY_H
