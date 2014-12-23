#ifndef QI_API_H
#define QI_API_H

#include <QtCore/qglobal.h>
#include <QPointer>
#include <QSharedPointer>
#include <QDebug>

#if defined(QT_ITEMS_LIBRARY)
#  define QI_EXPORT Q_DECL_EXPORT
#  define QI_EXPORT_TEMPLATE
#else
#  define QI_EXPORT Q_DECL_IMPORT
#  define QI_EXPORT_TEMPLATE extern
#endif

namespace Qi
{

static const int InvalidIndex = -1;
    
enum ChangeReasonFlag
{
    ChangeReasonLinesCount = 0x0001,
    ChangeReasonLinesCountWeak = 0x0002,
    ChangeReasonLinesVisibility = 0x0004,
    ChangeReasonLinesSize = 0x0008,
    ChangeReasonLinesOrder = 0x0010,
    
    ChangeReasonRange = 0x0020,
    ChangeReasonLayout = 0x0040,
    ChangeReasonViewContent = 0x0080,
    ChangeReasonViewController = 0x00F0,
    ChangeReasonViewSize = 0x0100,

    ChangeReasonSpaceStructure = 0x0200,
    ChangeReasonSpaceItemsStructure = 0x0400,
    ChangeReasonSpaceItemsContent = 0x0800,

    ChangeReasonCacheFrame = 0x1000,
    ChangeReasonCacheContent = 0x2000,
};

Q_DECLARE_FLAGS(ChangeReason, ChangeReasonFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(ChangeReason)

enum ViewSizeMode
{
    // calculate size exactly
    ViewSizeModeExact = 0,
    // calculate size using average symbol size
    ViewSizeModeFastAverage = 1,
    // calculate size using max symbol size
    ViewSizeModeFastMax = 2
};

}

#endif // QI_API_H
