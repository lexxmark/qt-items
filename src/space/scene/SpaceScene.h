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

#ifndef QI_SPACE_SCENE_H
#define QI_SPACE_SCENE_H

#include "space/Space.h"

namespace Qi
{

enum SpaceSceneHint
{
    SpaceSceneHintNone = 0x0000,
    SpaceSceneHintSameSchemasByType = 0x0001,
};

class QI_EXPORT SpaceScene: public Space
{
    Q_OBJECT
    Q_DISABLE_COPY(SpaceScene)

public:
    SpaceScene(SpaceSceneHint hint = SpaceSceneHintNone);
    ~SpaceScene();

    SpaceSceneHint hint() const { return m_hint; }
    void setHint(SpaceSceneHint hint);

    int count() const { return countImpl(); }

    QSize size() const override;
    ID toAbsolute(ID visibleId) const override { return visibleId; }
    ID toVisible(ID absoluteId) const override { return absoluteId; }
    QRect itemRect(ID visibleId) const override { return elementRectImpl(index(visibleId)); }
    SharedPtr<CacheItemFactory> createCacheItemFactory(ViewApplicationMask viewApplicationMask = ViewApplicationNone) const override;

    int itemType(int id) const { return elementTypeImpl(id); }

protected:
    virtual int countImpl() const = 0;
    virtual QRect elementRectImpl(int id) const = 0;
    virtual int elementTypeImpl(int id) const = 0;

    void notifyCountChanged();

private:
    SpaceSceneHint m_hint;
    mutable bool m_sizeIsValid;
    mutable QSize m_size;
};

QI_EXPORT SharedPtr<Range> makeRangeByType(const SpaceScene* scene, int type);


class SceneElement;

class QI_EXPORT SpaceSceneElements: public SpaceScene
{
    Q_OBJECT
    Q_DISABLE_COPY(SpaceSceneElements)

public:
    SpaceSceneElements(SpaceSceneHint hint = SpaceSceneHintNone);
    ~SpaceSceneElements();

    void addElement(SharedPtr<SceneElement> element);
    void clearElements();
    void setElements(QVector<SharedPtr<SceneElement> > elements);

protected:
    int countImpl() const override { return m_elements.size(); }
    QRect elementRectImpl(int id) const override;
    int elementTypeImpl(int id) const override;

private:
    QVector<SharedPtr<SceneElement>> m_elements;
};

enum SceneElementType
{
    SceneElementTypeNode = 0,
    SceneElementTypeAnchor = 1,
    SceneElementTypeConnection = 2,

    SceneElementTypeUser = 100
};

class QI_EXPORT SceneElement
{
    Q_DISABLE_COPY(SceneElement)

public:
    virtual ~SceneElement() {}

    QRect rect() const { return rectImpl(); }
    int type() const { return typeImpl(); }

protected:
    SceneElement() {}

    virtual QRect rectImpl() const = 0;
    virtual int typeImpl() const = 0;
};

class QI_EXPORT SceneElementNode: public SceneElement
{
    Q_DISABLE_COPY(SceneElementNode)

public:
    SceneElementNode(const QRect& rect, int type = SceneElementTypeNode)
        : m_rect(rect), m_type(type)
    {
    }

protected:
    QRect rectImpl() const override { return m_rect; }
    int typeImpl() const override { return m_type; }

private:
    QRect m_rect;
    int m_type;
};

class QI_EXPORT SceneElementPoint: public SceneElement
{
    Q_DISABLE_COPY(SceneElementPoint)

public:
    QPoint point() const { return pointImpl(); }

protected:
    SceneElementPoint() {}

    virtual QPoint pointImpl() const = 0;
};

enum AnchorFlag
{
    Left = 0x001,
    Right = 0x002,
    HCenter = 0x004,
    Top = 0x008,
    Bottom = 0x010,
    VCenter = 0x020
};
Q_DECLARE_FLAGS(Anchor, AnchorFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(Anchor)

class QI_EXPORT SceneElementAnchor: public SceneElementPoint
{
    Q_DISABLE_COPY(SceneElementAnchor)

public:
    SceneElementAnchor(SharedPtr<SceneElement> sourceElement, Anchor anchor, int type = SceneElementTypeAnchor);

protected:
    QRect rectImpl() const override;
    int typeImpl() const override { return m_type; }
    QPoint pointImpl() const override;

private:
    SharedPtr<SceneElement> m_sourceElement;
    Anchor m_anchor;
    int m_type;
};

class QI_EXPORT SceneElementConnection: public SceneElement
{
    Q_DISABLE_COPY(SceneElementConnection)

public:
    SceneElementConnection(SharedPtr<SceneElementPoint> elementFrom, SharedPtr<SceneElementPoint> elementTo, int type = SceneElementTypeConnection);

protected:
    QRect rectImpl() const override;
    int typeImpl() const override { return m_type; }

private:
    SharedPtr<SceneElementPoint> m_elementFrom;
    SharedPtr<SceneElementPoint> m_elementTo;
    int m_type;
};



} // end namespace Qi 

#endif // QI_SPACE_SCENE_H
