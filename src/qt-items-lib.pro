include(../common.pri)

QT += core gui widgets

TARGET = qt-items
TEMPLATE = lib
VERSION = 0.1.0

DEFINES += QT_ITEMS_LIBRARY

INCLUDEPATH += $$PWD
#message($$INCLUDEPATH)

SOURCES += QiAPI.cpp \
    core/Range.cpp \
    core/ext/Ranges.cpp \
    core/Layout.cpp \
    core/ext/Layouts.cpp \
    core/View.cpp \
    core/ext/Views.cpp \
    core/Model.cpp \
    core/ControllerMouse.cpp \
    core/ItemSchema.cpp \
    space/Lines.cpp \
    space/Space.cpp \
    space/SpaceGrid.cpp \
    widgets/ItemWidget.cpp \
    widgets/GridWidget.cpp \
    widgets/ListWidget.cpp \
    cache/CacheItem.cpp \
    cache/CacheView.cpp \
    space/SpaceItem.cpp \
    cache/space/CacheSpace.cpp \
    cache/space/CacheSpaceGrid.cpp \
    cache/space/CacheSpaceItem.cpp \
    core/ext/ViewComposite.cpp \
    items/cache/ViewCacheSpace.cpp \
    core/ext/ControllerMouseMultiple.cpp \
    cache/CacheControllerMouse.cpp \
    core/ext/ControllerMouseCaptured.cpp \
    core/ext/ControllerMousePushable.cpp \
    items/checkbox/Check.cpp \
    items/radiobutton/Radio.cpp \
    items/text/Text.cpp \
    items/selection/Selection.cpp \
    core/ControllerKeyboard.cpp \
    items/misc/ControllerMousePushableCallback.cpp \
    items/button/Button.cpp \
    items/image/StyleStandardPixmap.cpp \
    items/selection/SelectionIterators.cpp \
    items/image/Pixmap.cpp \
    items/image/Image.cpp \
    items/link/Link.cpp \
    items/progressbar/Progress.cpp \
    items/color/Color.cpp \
    core/ext/LayoutsAux.cpp \
    items/misc/ViewItemBorder.cpp \
    items/misc/ViewAlternateBackground.cpp \
    items/misc/ControllerMouseLinesResizer.cpp \
    core/misc/ControllerMouseAuxiliary.cpp \
    items/numeric/Numeric.cpp \
    items/enum/Enum.cpp \
    items/sorting/Sorting.cpp \
    items/visible/Visible.cpp \
    items/filter/Filter.cpp \
    items/filter/FilterText.cpp \
    utils/InplaceEditing.cpp \
    core/ext/ControllerMouseInplaceEdit.cpp \
    cache/CacheItemFactory.cpp \
    widgets/core/SpaceWidgetAbstract.cpp \
    widgets/core/SpaceWidgetCore.cpp \
    widgets/core/SpaceWidgetScrollAbstract.cpp \
    misc/GridColumnsResizer.cpp \
    utils/CallLater.cpp \
    misc/CacheSpaceAnimation.cpp \
    space/SpaceScene.cpp \
    widgets/SceneWidget.cpp \
    cache/space/CacheSpaceScene.cpp \
    items/rating/Rating.cpp \
    utils/PainterState.cpp

HEADERS +=  QiAPI.h \
    utils/Signal.h \
    core/ItemID.h \
    core/Range.h \
    core/ext/Ranges.h \
    core/Layout.h \
    core/ext/Layouts.h \
    core/View.h \
    core/ext/Views.h \
    core/Model.h \
    core/ControllerMouse.h \
    core/ItemSchema.h \
    space/Lines.h \
    space/Space.h \
    space/SpaceGrid.h \
    widgets/ItemWidget.h \
    widgets/GridWidget.h \
    widgets/ListWidget.h \
    cache/CacheItem.h \
    cache/CacheView.h \
    space/SpaceItem.h \
    cache/space/CacheSpace.h \
    cache/space/CacheSpaceGrid.h \
    utils/auto_value.h \
    cache/space/CacheSpaceItem.h \
    core/ext/ViewComposite.h \
    core/ext/ModelTyped.h \
    core/ItemsIterator.h \
    core/ext/ModelStore.h \
    core/ext/ModelCallback.h \
    core/ext/ModelConversion.h \
    items/cache/ViewCacheSpace.h \
    core/ext/ControllerMouseMultiple.h \
    cache/CacheControllerMouse.h \
    core/ext/ControllerMouseCaptured.h \
    core/ext/ControllerMousePushable.h \
    core/ext/ViewModeled.h \
    items/checkbox/Check.h \
    items/radiobutton/Radio.h \
    items/text/Text.h \
    items/selection/Selection.h \
    core/ControllerKeyboard.h \
    items/misc/ControllerMousePushableCallback.h \
    items/button/Button.h \
    items/image/StyleStandardPixmap.h \
    items/selection/SelectionIterators.h \
    items/image/Pixmap.h \
    items/image/Image.h \
    items/link/Link.h \
    items/progressbar/Progress.h \
    items/color/Color.h \
    core/ext/LayoutsAux.h \
    items/misc/ViewItemBorder.h \
    items/misc/ViewAlternateBackground.h \
    items/misc/ControllerMouseLinesResizer.h \
    core/misc/ViewAuxiliary.h \
    core/misc/ControllerMouseAuxiliary.h \
    items/numeric/Numeric.h \
    items/enum/Enum.h \
    items/sorting/Sorting.h \
    items/visible/Visible.h \
    items/filter/Filter.h \
    items/filter/FilterText.h \
    utils/InplaceEditing.h \
    core/ext/ControllerMouseInplaceEdit.h \
    cache/CacheItemFactory.h \
    widgets/core/SpaceWidgetAbstract.h \
    widgets/core/SpaceWidgetCore.h \
    widgets/core/SpaceWidgetScrollAbstract.h \
    misc/GridColumnsResizer.h \
    utils/CallLater.h \
    utils/MemFunction.h \
    misc/CacheSpaceAnimation.h \
    space/SpaceScene.h \
    widgets/SceneWidget.h \
    cache/space/CacheSpaceScene.h \
    items/rating/Rating.h \
    utils/PainterState.h

win32 {
    TARGET_EXT = .dll
} else:unix {
    target.path = /usr/lib
    INSTALLS += target
}
