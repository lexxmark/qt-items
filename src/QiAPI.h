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
    ChangeReasonLinesCount = 0x00001,
    ChangeReasonLinesCountWeak = 0x00002,
    ChangeReasonLinesVisibility = 0x00004,
    ChangeReasonLinesSize = 0x00008,
    ChangeReasonLinesOrder = 0x00010,
    
    ChangeReasonRange = 0x00020,
    ChangeReasonLayout = 0x00040,
    ChangeReasonViewContent = 0x00080,
    ChangeReasonViewController = 0x00100,
    ChangeReasonViewSize = 0x00200,

    ChangeReasonSpaceStructure = 0x00400,
    ChangeReasonSpaceHint = 0x00800,
    ChangeReasonSpaceItemsStructure = 0x01000,
    ChangeReasonSpaceItemsContent = 0x02000,

    ChangeReasonCacheItems = 0x04000,
    ChangeReasonCacheContent = 0x08000,
    ChangeReasonCacheFrame = 0x10000,
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
