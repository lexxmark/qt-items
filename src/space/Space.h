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

#ifndef QI_SPACE_H
#define QI_SPACE_H

#include "core/ItemSchema.h"
#include "core/Layout.h"
#include "core/View.h"
#include <QSize>
#include <QPoint>

namespace Qi
{

class CacheItemFactory;

class QI_EXPORT Space: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Space)

public:
    Space();
    virtual ~Space();

    virtual QSize size() const = 0;
    virtual ItemID toAbsolute(const ItemID& visibleItem) const = 0;
    virtual ItemID toVisible(const ItemID& absoluteItem) const = 0;
    virtual QRect itemRect(const ItemID& visibleItem) const = 0;
    virtual QSharedPointer<CacheItemFactory> createCacheItemFactory(ViewApplicationMask viewApplicationMask = ViewApplicationNone) const = 0;

    const QVector<ItemSchema>& schemas() const { return m_schemas; }
    int addSchema(const ItemSchema& schema);
    int addSchema(const QSharedPointer<Range>& range, const QSharedPointer<View>& view, const QSharedPointer<Layout>& layout = makeLayoutClient()) { return addSchema(ItemSchema(range, layout, view)); }
    int insertSchema(int index, const QSharedPointer<Range>& range, const QSharedPointer<View>& view, const QSharedPointer<Layout>& layout = makeLayoutClient());
    void removeSchema(const QSharedPointer<View>& view);
    void clearSchemas();

    ViewApplicationMask viewApplicationMask() const { return m_viewApplicationMask; }
    void setViewApplicationMask(ViewApplicationMask viewApplicationMask);

    const QVector<ItemSchema>& schemasOrdered() const;

signals:
    void spaceChanged(const Space* space, ChangeReason reason);

private slots:
    void onRangeChanged(const Range* range, ChangeReason reason);
    void onLayoutChanged(const Layout* layout, ChangeReason reason);
    void onViewChanged(const View* view, ChangeReason reason);

private:
    void connectSchema(const ItemSchema& schema);
    void disconnectSchema(const ItemSchema& schema);

    QVector<ItemSchema> m_schemas;
    mutable QVector<ItemSchema> m_schemasOrdered;

    // views filtering
    ViewApplicationMask m_viewApplicationMask;
};

} // end namespace Qi 

#endif // QI_GRID_SPACE_H
