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
#include "space/grid/GridID.h"

namespace Qi
{

CacheItemFactory::CacheItemFactory(const Space2& space)
    : m_space(space)
{
}

CacheItemFactory::~CacheItemFactory()
{
}

CacheItemInfo CacheItemFactory::create(ID visibleId) const
{
    CacheItemInfo info(m_space.toAbsolute(visibleId));
    updateSchema(info);
    info.rect = m_space.itemRect(visibleId);

    return info;
}

void CacheItemFactory::updateSchema(CacheItemInfo& info) const
{
    initSchemaImpl(info);
}

void CacheItemFactory::initSchemaImpl(CacheItemInfo& info) const
{
    info.schema = createViewSchema(info.id);
}

ViewSchema CacheItemFactory::createViewSchema(ID absId) const
{
    QVector<ViewSchema> viewSchemas;

    for (const auto& schema : m_space.schemasOrdered())
    {
        if (schema.range->hasItem(absId))
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
        schema.view = makeShared<ViewComposite>(viewSchemas);
        return schema;
    }
}

SharedPtr<CacheItemFactory> createCacheItemFactoryDefault(const Space2& space)
{
    return makeShared<CacheItemFactory>(space);
}

class CacheItemFactoryItem: public CacheItemFactory
{
public:
    CacheItemFactoryItem(const Space2& space)
        : CacheItemFactory(space)
    {}

protected:
    void initSchemaImpl(CacheItemInfo& info) const
    {
        if (!m_initialized || m_id != info.id)
        {
            m_schema = createViewSchema(info.id);
            m_id = info.id;
            m_initialized = true;
        }

        info.schema = m_schema;
    }

private:
    mutable bool m_initialized = false;
    mutable ID m_id;
    mutable ViewSchema m_schema;
};

SharedPtr<CacheItemFactory> createCacheItemFactoryItem(const Space2& space)
{
    return makeShared<CacheItemFactoryItem>(space);
}

class CacheItemFactorySameSchemaByColumn: public CacheItemFactory
{
public:
    CacheItemFactorySameSchemaByColumn(const Space2& space)
        : CacheItemFactory(space)
    {}

protected:
    void initSchemaImpl(CacheItemInfo& info) const
    {
        auto it = m_schemaByColumn.find(column(info.id));
        if (it != m_schemaByColumn.end())
        {
            info.schema = it.value();
        }
        else
        {
            ViewSchema& schema = m_schemaByColumn[column(info.id)];
            schema = createViewSchema(info.id);
            info.schema = schema;
        }
    }

private:
    mutable QMap<int, ViewSchema> m_schemaByColumn;
};

SharedPtr<CacheItemFactory> createCacheItemFactorySameSchemaByColumn(const Space2& space)
{
    return makeShared<CacheItemFactorySameSchemaByColumn>(space);
}

class CacheItemFactorySameSchemaByRow: public CacheItemFactory
{
public:
    CacheItemFactorySameSchemaByRow(const Space2& space)
        : CacheItemFactory(space)
    {}

protected:
    void initSchemaImpl(CacheItemInfo& info) const
    {
        auto it = m_schemaByRow.find(row(info.id));
        if (it != m_schemaByRow.end())
        {
            info.schema = it.value();
        }
        else
        {
            ViewSchema& schema = m_schemaByRow[row(info.id)];
            schema = createViewSchema(info.id);
            info.schema = schema;
        }
    }

private:
    mutable QMap<int, ViewSchema> m_schemaByRow;
};

SharedPtr<CacheItemFactory> createCacheItemFactorySameSchemaByRow(const Space2& space)
{
    return makeShared<CacheItemFactorySameSchemaByRow>(space);
}


} // end namespace Qi
