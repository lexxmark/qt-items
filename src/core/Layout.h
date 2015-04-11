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

#ifndef QI_LAYOUT_H
#define QI_LAYOUT_H

#include "ItemID.h"
#include <QRect>

namespace Qi
{

class View;
class GuiContext;

enum LayoutBehavior
{
    LayoutBehaviorNone = 0x00,
    LayoutBehaviorTransparent = 0x01,
    LayoutBehaviorFloat = 0x02,
};
typedef quint16 LayoutBehaviorMask;

// view layout interface
class QI_EXPORT Layout: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Layout)

public:
    virtual ~Layout() {}

    // perform layout
    bool doLayout(const View& view, const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode, QRect& viewRect, QRect& itemRect, QRect* visibleItemRect) const;
    // expand size
    void expandSize(const View& view, const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode, QSize& size) const;
    // is final (eats all available item space)
    bool isFinal() const;

    LayoutBehaviorMask behavior() const { return m_behavior; }
    bool isTransparent() const { return m_behavior&LayoutBehaviorTransparent; }
    bool isFloat() const { return m_behavior&LayoutBehaviorFloat; }

signals:
    void layoutChanged(const Layout*, ChangeReason);

protected:
    Layout(LayoutBehaviorMask behavior)
        : m_behavior(behavior)
    {}

    class QI_EXPORT ViewInfo
    {
    public:
        ViewInfo(const View& view, const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode)
            : view(view), ctx(ctx), item(item), sizeMode(sizeMode)
        {}

        QSize size() const;

        const View& view;
        const GuiContext& ctx;
        const ItemID& item;
        ViewSizeMode sizeMode;
    };

    class QI_EXPORT LayoutInfo
    {
    public:
        LayoutInfo(QRect& viewRect, QRect& itemRect)
            : viewRect(viewRect), itemRect(itemRect)
        {}

        QRect& viewRect;
        QRect& itemRect;
    };

    // perform layout
    virtual bool doLayoutImpl(const ViewInfo& viewInfo, LayoutInfo& info) const = 0;
    // expand size
    virtual void expandSizeImpl(const ViewInfo& viewInfo, QSize& size) const = 0;
    // is final
    virtual bool isFinalImpl() const { return false; }

private:
    LayoutBehaviorMask m_behavior;
};

QI_EXPORT QSharedPointer<Layout> makeLayoutBackground();
QI_EXPORT QSharedPointer<Layout> makeLayoutClient();
QI_EXPORT QSharedPointer<Layout> makeLayoutCenter();
QI_EXPORT QSharedPointer<Layout> makeLayoutLeft(LayoutBehaviorMask behavior = LayoutBehaviorNone);
QI_EXPORT QSharedPointer<Layout> makeLayoutRight(LayoutBehaviorMask behavior = LayoutBehaviorNone);
QI_EXPORT QSharedPointer<Layout> makeLayoutTop(LayoutBehaviorMask behavior = LayoutBehaviorNone);
QI_EXPORT QSharedPointer<Layout> makeLayoutBottom(LayoutBehaviorMask behavior = LayoutBehaviorNone);
QI_EXPORT QSharedPointer<Layout> makeLayoutSquareLeft(LayoutBehaviorMask behavior = LayoutBehaviorNone);
QI_EXPORT QSharedPointer<Layout> makeLayoutSquareRight(LayoutBehaviorMask behavior = LayoutBehaviorNone);
QI_EXPORT QSharedPointer<Layout> makeLayoutSquareTop(LayoutBehaviorMask behavior = LayoutBehaviorNone);
QI_EXPORT QSharedPointer<Layout> makeLayoutSquareBottom(LayoutBehaviorMask behavior = LayoutBehaviorNone);
QI_EXPORT QSharedPointer<Layout> makeLayoutFixedLeft(int width, LayoutBehaviorMask behavior = LayoutBehaviorNone);
QI_EXPORT QSharedPointer<Layout> makeLayoutFixedRight(int width, LayoutBehaviorMask behavior = LayoutBehaviorNone);
QI_EXPORT QSharedPointer<Layout> makeLayoutFixedTop(int height, LayoutBehaviorMask behavior = LayoutBehaviorNone);
QI_EXPORT QSharedPointer<Layout> makeLayoutFixedBottom(int height, LayoutBehaviorMask behavior = LayoutBehaviorNone);

} // end namespace Qi

#endif // QI_LAYOUT_H
