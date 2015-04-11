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

#include "CacheItemFactory.h"
#include "core/ext/Layouts.h"
#include "core/ext/ViewComposite.h"

namespace Qi
{

CacheItemFactory::CacheItemFactory(const Space& space, ViewApplicationMask viewApplicationMask)
    : m_space(space),
      m_viewApplicationMask(viewApplicationMask)
{
}

CacheItemFactory::~CacheItemFactory()
{
}

CacheItemInfo CacheItemFactory::create(const ItemID& visibleItem) const
{
    CacheItemInfo info;
    info.item = m_space.toAbsolute(visibleItem);
    updateSchema(info);
    info.rect = m_space.itemRect(visibleItem);

    return info;
}

void CacheItemFactory::updateSchema(CacheItemInfo& info) const
{
    Q_ASSERT(info.item.isValid());
    initSchemaImpl(info);
}

void CacheItemFactory::initSchemaImpl(CacheItemInfo& info) const
{
    info.schema = createViewSchema(info.item);
}

ViewSchema CacheItemFactory::createViewSchema(const ItemID& absItem) const
{
    QVector<ViewSchema> viewSchemas;
    auto viewApplicationMask = m_space.viewApplicationMask() | m_viewApplicationMask;

    for (const auto& schema : m_space.schemasOrdered())
    {
        if (schema.range->hasItem(absItem) && schema.view->isApplicable(viewApplicationMask))
        {
            viewSchemas.append(ViewSchema(schema.layout, schema.view));
        }
    }

    if (viewSchemas.empty())
        return ViewSchema();
    else if (viewSchemas.size() == 1)
        return viewSchemas.front();
    else
    {
        ViewSchema schema;
        schema.layout = makeLayoutBackground();
        schema.view = QSharedPointer<ViewComposite>::create(viewSchemas);
        return schema;
    }
}

QSharedPointer<CacheItemFactory> createCacheItemFactoryDefault(const Space& space, ViewApplicationMask viewApplicationMask)
{
    return QSharedPointer<CacheItemFactory>::create(space, viewApplicationMask);
}

class CacheItemFactoryItem: public CacheItemFactory
{
public:
    CacheItemFactoryItem(const Space& space, ViewApplicationMask viewApplicationMask)
        : CacheItemFactory(space, viewApplicationMask)
    {}

protected:
    void initSchemaImpl(CacheItemInfo& info) const
    {
        if (m_item != info.item)
        {
            m_schema = createViewSchema(info.item);
            m_item = info.item;
        }

        info.schema = m_schema;
    }

private:
    mutable ItemID m_item;
    mutable ViewSchema m_schema;
};

QSharedPointer<CacheItemFactory> createCacheItemFactoryItem(const Space& space, ViewApplicationMask viewApplicationMask)
{
    return QSharedPointer<CacheItemFactoryItem>::create(space, viewApplicationMask);
}

class CacheItemFactorySameSchemaByColumn: public CacheItemFactory
{
public:
    CacheItemFactorySameSchemaByColumn(const Space& space, ViewApplicationMask viewApplicationMask)
        : CacheItemFactory(space, viewApplicationMask)
    {}

protected:
    void initSchemaImpl(CacheItemInfo& info) const
    {
        auto it = m_schemaByColumn.find(info.item.column);
        if (it != m_schemaByColumn.end())
        {
            info.schema = it.value();
        }
        else
        {
            ViewSchema& schema = m_schemaByColumn[info.item.column];
            schema = createViewSchema(info.item);
            info.schema = schema;
        }
    }

private:
    mutable QMap<int, ViewSchema> m_schemaByColumn;
};

QSharedPointer<CacheItemFactory> createCacheItemFactorySameSchemaByColumn(const Space& space, ViewApplicationMask viewApplicationMask)
{
    return QSharedPointer<CacheItemFactorySameSchemaByColumn>::create(space, viewApplicationMask);
}

class CacheItemFactorySameSchemaByRow: public CacheItemFactory
{
public:
    CacheItemFactorySameSchemaByRow(const Space& space, ViewApplicationMask viewApplicationMask)
        : CacheItemFactory(space, viewApplicationMask)
    {}

protected:
    void initSchemaImpl(CacheItemInfo& info) const
    {
        auto it = m_schemaByRow.find(info.item.row);
        if (it != m_schemaByRow.end())
        {
            info.schema = it.value();
        }
        else
        {
            ViewSchema& schema = m_schemaByRow[info.item.row];
            schema = createViewSchema(info.item);
            info.schema = schema;
        }
    }

private:
    mutable QMap<int, ViewSchema> m_schemaByRow;
};

QSharedPointer<CacheItemFactory> createCacheItemFactorySameSchemaByRow(const Space& space, ViewApplicationMask viewApplicationMask)
{
    return QSharedPointer<CacheItemFactorySameSchemaByRow>::create(space, viewApplicationMask);
}


} // end namespace Qi
