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
    core/Layout.cpp \
    core/View.cpp \
    core/Model.cpp \
    core/ControllerMouse.cpp \
    core/ControllerKeyboard.cpp \
    core/ItemSchema.cpp \
    core/ext/Layouts.cpp \
    core/ext/Views.cpp \
    core/ext/Ranges.cpp \
    core/ext/LayoutsAux.cpp \
    core/ext/ViewComposite.cpp \
    core/ext/ControllerMouseMultiple.cpp \
    core/ext/ControllerMouseCaptured.cpp \
    core/ext/ControllerMousePushable.cpp \
    core/ext/ControllerMouseInplaceEdit.cpp \
    core/misc/ControllerMouseAuxiliary.cpp \
    space/Space.cpp \
    space/CacheSpace.cpp \
    space/grid/Lines.cpp \
    space/grid/SpaceGrid.cpp \
    space/grid/RangeGrid.cpp \
    space/grid/CacheSpaceGrid.cpp \
    space/item/SpaceItem.cpp \
    space/item/CacheSpaceItem.cpp \
    space/scene/SpaceScene.cpp \
    space/scene/CacheSpaceScene.cpp \
    cache/CacheItem.cpp \
    cache/CacheView.cpp \
    cache/CacheControllerMouse.cpp \
    cache/CacheItemFactory.cpp \
    items/cache/ViewCacheSpace.cpp \
    items/checkbox/Check.cpp \
    items/radiobutton/Radio.cpp \
    items/text/Text.cpp \
    items/selection/Selection.cpp \
    items/misc/ControllerMousePushableCallback.cpp \
    items/button/Button.cpp \
    items/image/StyleStandardPixmap.cpp \
    items/selection/SelectionIterators.cpp \
    items/image/Pixmap.cpp \
    items/image/Image.cpp \
    items/link/Link.cpp \
    items/progressbar/Progress.cpp \
    items/color/Color.cpp \
    items/misc/ViewItemBorder.cpp \
    items/misc/ViewAlternateBackground.cpp \
    items/misc/ControllerMouseLinesResizer.cpp \
    items/numeric/Numeric.cpp \
    items/enum/Enum.cpp \
    items/sorting/Sorting.cpp \
    items/visible/Visible.cpp \
    items/filter/Filter.cpp \
    items/filter/FilterText.cpp \
    items/rating/Rating.cpp \
    widgets/ItemWidget.cpp \
    widgets/GridWidget.cpp \
    widgets/ListWidget.cpp \
    widgets/SceneWidget.cpp \
    widgets/core/SpaceWidgetAbstract.cpp \
    widgets/core/SpaceWidgetCore.cpp \
    widgets/core/SpaceWidgetScrollAbstract.cpp \
    misc/GridColumnsResizer.cpp \
    misc/CacheSpaceAnimation.cpp \
    utils/PainterState.cpp \
    utils/InplaceEditing.cpp \
    utils/CallLater.cpp

HEADERS +=  QiAPI.h \
    core/ID.h \
    core/Range.h \
    core/Layout.h \
    core/View.h \
    core/Model.h \
    core/ControllerMouse.h \
    core/ItemSchema.h \
    core/ItemsIterator.h \
    core/ControllerKeyboard.h \
    cache/CacheItemFactory.h \
    core/ext/LayoutsAux.h \
    core/ext/Ranges.h \
    core/ext/Views.h \
    core/ext/Layouts.h \
    core/ext/ControllerMouseInplaceEdit.h \
    core/ext/ViewComposite.h \
    core/ext/ModelTyped.h \
    core/ext/ModelStore.h \
    core/ext/ModelCallback.h \
    core/ext/ModelConversion.h \
    core/ext/ControllerMouseMultiple.h \
    core/ext/ControllerMouseCaptured.h \
    core/ext/ControllerMousePushable.h \
    core/ext/ViewModeled.h \
    core/misc/ViewAuxiliary.h \
    core/misc/ControllerMouseAuxiliary.h \
    space/Space.h \
    space/CacheSpace.h \
    space/grid/Lines.h \
    space/grid/SpaceGrid.h \
    space/grid/CacheSpaceGrid.h \
    space/grid/GridID.h \
    space/grid/RangeGrid.h \
    space/item/SpaceItem.h \
    space/item/CacheSpaceItem.h \
    space/scene/CacheSpaceScene.h \
    space/scene/SpaceScene.h \
    cache/CacheItem.h \
    cache/CacheView.h \
    cache/CacheControllerMouse.h \
    items/checkbox/Check.h \
    items/radiobutton/Radio.h \
    items/text/Text.h \
    items/selection/Selection.h \
    items/cache/ViewCacheSpace.h \
    items/misc/ControllerMousePushableCallback.h \
    items/button/Button.h \
    items/image/StyleStandardPixmap.h \
    items/selection/SelectionIterators.h \
    items/image/Pixmap.h \
    items/image/Image.h \
    items/link/Link.h \
    items/progressbar/Progress.h \
    items/color/Color.h \
    items/misc/ViewItemBorder.h \
    items/misc/ViewAlternateBackground.h \
    items/misc/ControllerMouseLinesResizer.h \
    items/numeric/Numeric.h \
    items/enum/Enum.h \
    items/sorting/Sorting.h \
    items/visible/Visible.h \
    items/filter/Filter.h \
    items/filter/FilterText.h \
    items/rating/Rating.h \
    widgets/ItemWidget.h \
    widgets/GridWidget.h \
    widgets/ListWidget.h \
    widgets/SceneWidget.h \
    widgets/core/SpaceWidgetAbstract.h \
    widgets/core/SpaceWidgetCore.h \
    widgets/core/SpaceWidgetScrollAbstract.h \
    misc/GridColumnsResizer.h \
    misc/CacheSpaceAnimation.h \
    utils/CallLater.h \
    utils/MemFunction.h \
    utils/PainterState.h \
    utils/InplaceEditing.h \
    utils/auto_value.h

win32 {
    TARGET_EXT = .dll
} else:unix {
    target.path = /usr/lib
    INSTALLS += target
}
