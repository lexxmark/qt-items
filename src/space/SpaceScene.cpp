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

#include "SpaceScene.h"
#include "cache/CacheItemFactory.h"

namespace Qi
{

class RangeByType: public Range
{
public:
    RangeByType(const SpaceScene* scene, int type)
        : m_scene(scene),
          m_type(type)
    {
    }

protected:
    bool hasItemImpl(const ItemID &item) const override
    {
        return m_scene->itemType(item) == m_type;
    }
    bool hasRowImpl(int /*row*/) const override { return false; }
    bool hasColumnImpl(int /*column*/) const { return false; }

private:
    const SpaceScene* m_scene;
    int m_type;
};

QSharedPointer<Range> makeRangeByType(const SpaceScene* scene, int type)
{
    return QSharedPointer<RangeByType>::create(scene, type);
}

class CacheItemFactorySameSchemaByType: public CacheItemFactory
{
public:
    CacheItemFactorySameSchemaByType(const SpaceScene& space, ViewApplicationMask viewApplicationMask)
        : CacheItemFactory(space, viewApplicationMask),
          m_spaceScene(space)
    {}

protected:
    void initSchemaImpl(CacheItemInfo& info) const
    {
        auto it = m_schemaByType.find(m_spaceScene.itemType(info.item));
        if (it != m_schemaByType.end())
        {
            info.schema = it.value();
        }
        else
        {
            ViewSchema& schema = m_schemaByType[info.item.column];
            schema = createViewSchema(info.item);
            info.schema = schema;
        }
    }

private:
    const SpaceScene& m_spaceScene;
    mutable QMap<int, ViewSchema> m_schemaByType;
};

SpaceScene::SpaceScene(SpaceSceneHint hint)
    : m_hint(hint),
      m_sizeIsValid(false)
{
}

SpaceScene::~SpaceScene()
{
}

void SpaceScene::setHint(SpaceSceneHint hint)
{
    if (m_hint == hint)
        return;

    m_hint = hint;
    emit spaceChanged(this, ChangeReasonSpaceHint);
}

QSize SpaceScene::size() const
{
    if (m_sizeIsValid)
        return m_size;

    m_size = QSize(0, 0);
    int count = countImpl();
    for (ItemID item(0, 0); item.column < count; ++item.column)
    {
        QRect rect = itemRect(item);
        m_size.rwidth() = qMax(m_size.width(), rect.right());
        m_size.rheight() = qMax(m_size.height(), rect.bottom());
    }

    m_sizeIsValid = true;
    return m_size;
}

QSharedPointer<CacheItemFactory> SpaceScene::createCacheItemFactory(ViewApplicationMask viewApplicationMask) const
{
    switch (m_hint) {
    case SpaceSceneHintSameSchemasByType:
        return QSharedPointer<CacheItemFactory>(new CacheItemFactorySameSchemaByType(*this, viewApplicationMask));
    default:
        return createCacheItemFactoryDefault(*this, viewApplicationMask);
    }
}

void SpaceScene::notifyCountChanged()
{
    m_sizeIsValid = false;
    emit spaceChanged(this, ChangeReasonSpaceHint);
}

SpaceSceneElements::SpaceSceneElements(SpaceSceneHint hint)
    : SpaceScene(hint)
{
}

SpaceSceneElements::~SpaceSceneElements()
{
}

void SpaceSceneElements::addElement(const QSharedPointer<SceneElement>& element)
{
    m_elements.append(element);
    notifyCountChanged();
}

void SpaceSceneElements::clearElements()
{
    m_elements.clear();
    notifyCountChanged();
}

void SpaceSceneElements::setElements(const QVector<QSharedPointer<SceneElement>>& elements)
{
    m_elements = elements;
    notifyCountChanged();
}

QRect SpaceSceneElements::elementRectImpl(const ItemID& item) const
{
    return m_elements[item.column]->rect();
}

int SpaceSceneElements::elementTypeImpl(const ItemID& item) const
{
    return m_elements[item.column]->type();
}

SceneElementAnchor::SceneElementAnchor(const QSharedPointer<SceneElement>& sourceElement, Anchor anchor, int type)
    : m_sourceElement(sourceElement),
      m_anchor(anchor),
      m_type(type)
{
}

QRect SceneElementAnchor::rectImpl() const
{
    return QRect(point(), QSize(1, 1));
}

QPoint SceneElementAnchor::pointImpl() const
{
    QRect rect = m_sourceElement->rect();

    QPoint point = rect.topLeft();

    if (m_anchor & Right)
        point.rx() = rect.right();
    else if (m_anchor & HCenter)
        point.rx() = rect.center().x();

    if (m_anchor & Bottom)
        point.ry() = rect.bottom();
    else if (m_anchor & VCenter)
        point.ry() = rect.center().y();

    return point;
}

SceneElementConnection::SceneElementConnection(const QSharedPointer<SceneElementPoint>& elementFrom, const QSharedPointer<SceneElementPoint>& elementTo, int type)
    : m_elementFrom(elementFrom),
      m_elementTo(elementTo),
      m_type(type)
{
}

QRect SceneElementConnection::rectImpl() const
{
    return QRect(m_elementFrom->point(), m_elementTo->point());
}

} // end namespace Qi
