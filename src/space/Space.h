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

#ifndef QI_Space2_H
#define QI_Space2_H

#include "core/ItemSchema.h"
#include <core/ItemsIterator.h>
#include <core/ext/ModelTyped.h>
#include <cache/CacheView.h>
#include <QRect>
#include <QSize>
#include <QPoint>

namespace Qi
{

class CacheItemFactory;

class QI_EXPORT Space2: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Space2)

public:
    Space2();
    virtual ~Space2();

    virtual QSize size() const = 0;
    virtual ID toAbsolute(ID visibleItem) const = 0;
    virtual ID toVisible(ID absoluteItem) const = 0;
    virtual QRect itemRect(ID visibleItem) const = 0;
    virtual SharedPtr<CacheItemFactory> createCacheItemFactory() const = 0;

    const QVector<ItemSchema>& schemas() const { return m_schemas; }
    int addSchema(const ItemSchema& schema);
    int addSchema(SharedPtr<Range> range, SharedPtr<View2> view, SharedPtr<Layout> layout = makeLayoutClient()) { return addSchema(ItemSchema(range, layout, view)); }
    int insertSchema(int index, SharedPtr<Range> range, SharedPtr<View2> view, SharedPtr<Layout> layout = makeLayoutClient());
    void removeSchema(SharedPtr<View2> view);
    void clearSchemas();

    const QVector<ItemSchema>& schemasOrdered() const;

signals:
    void spaceChanged(const Space2* Space2, ChangeReason reason);

private slots:
    void onRangeChanged(const Range* range, ChangeReason reason);
    void onLayoutChanged(const Layout* layout, ChangeReason reason);
    void onViewChanged(const View2* view, ChangeReason reason);

private:
    void connectSchema(const ItemSchema& schema);
    void disconnectSchema(const ItemSchema& schema);

    QVector<ItemSchema> m_schemas;
    mutable QVector<ItemSchema> m_schemasOrdered;
};

class QI_EXPORT Topology: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Topology)

public:
    Topology() = default;
    virtual ~Topology() = default;

    QRect boundBox() const { return boundBoxImpl(); }
    QRect itemRect(ID id) const { return itemRectImpl(id); }
    SharedPtr<IdIterator> items(const QRect* rect = nullptr) const { return itemsImpl(rect); }

protected:
    virtual QRect boundBoxImpl() const = 0;
    virtual QRect itemRectImpl(ID id) const = 0;
    virtual SharedPtr<IdIterator> itemsImpl(const QRect* rect) const = 0;

signals:
    void topologyChanged(const Topology* topology, ChangeReason reason);
};

class QI_EXPORT Schema: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Schema)

public:
    Schema() = default;
    virtual ~Schema() = default;

    SharedPtr<View> createCacheView(View* parent, ID id) const
    { return createCacheViewImpl(parent, id); }

protected:
   virtual SharedPtr<View> createCacheViewImpl(View* parent, ID id) const = 0;

signals:
    void schemaChanged(const Schema* schema, ChangeReason reason);
};

struct QI_EXPORT Space
{
    SharedPtr<Topology> topology;
    SharedPtr<Schema> schema;
};

using ModelSpace = ModelTyped<SharedPtr<Space>>;

} // end namespace Qi

#endif // QI_GRID_Space2_H
