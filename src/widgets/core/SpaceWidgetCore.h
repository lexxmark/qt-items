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

#ifndef QI_SPACE_WIDGET_CORE_H
#define QI_SPACE_WIDGET_CORE_H

#include "core/misc/ViewAuxiliary.h"

#include <QMetaObject>

class QWidget;
class QKeyEvent;

namespace Qi
{

class ID;
class CacheSpace;
class Space;
class CacheControllerMouse;
class ControllerKeyboard;

class QI_EXPORT SpaceWidgetCore
{
    Q_DISABLE_COPY(SpaceWidgetCore)

public:
    const CacheSpace& mainCacheSpace() const { return *m_mainCacheSpace; }
    const Space& mainSpace() const;

    CacheSpace& rMainCacheSpace() { return *m_mainCacheSpace; }
    Space& rMainSpace();

    const SharedPtr<ControllerKeyboard>& controllerKeyboard() const { return m_controllerKeyboard; }
    void setControllerKeyboard(SharedPtr<ControllerKeyboard> controllerKeyboard);
    void addControllerKeyboard(SharedPtr<ControllerKeyboard> controllerKeyboard);

    // scrolls widget to make visibleItem fully visible
    void ensureVisible(ID visibleItem, const CacheSpace* cacheSpace, bool validateItem);
    // performs inplace editing for visibleItem
    bool doInplaceEdit(ID visibleItem, const CacheSpace* cacheSpace, const QKeyEvent* event);

    // context which used for draw operations
    const GuiContext& guiContext() const { return m_guiContext; }

    QPixmap createPixmap() const { return createPixmapImpl(); }

protected:
    explicit SpaceWidgetCore(QWidget* owner);
    ~SpaceWidgetCore();

    bool initSpaceWidgetCore(SharedPtr<CacheSpace> mainCacheSpace);

    bool processOwnerEvent(QEvent* event);

    void stopControllers();
    void resumeControllers();

    // scrolls widget to make visibleItem fully visible
    virtual void ensureVisibleImpl(const ID& visibleItem, const CacheSpace *cacheSpace, bool validateItem) = 0;
    // creates image of the widget
    virtual QPixmap createPixmapImpl() const;

private:
    void onCacheSpaceChanged(const CacheSpace* cache, ChangeReason reason);

    QWidget* m_owner;

    SharedPtr<CacheSpace> m_mainCacheSpace;
    UniquePtr<CacheControllerMouse> m_cacheControllers;
    SharedPtr<ControllerKeyboard> m_controllerKeyboard;

    GuiContext m_guiContext;

    QMetaObject::Connection m_connection;

#if !defined(QT_NO_DEBUG)
    QPointer<QWidget> m_trackOwner;
#endif
};

} // end namespace Qi

#endif // QI_SPACE_WIDGET_P_H
