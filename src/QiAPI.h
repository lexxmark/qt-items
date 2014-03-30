#ifndef QI_API_H
#define QI_API_H
#include <QtCore/qglobal.h>

#if defined(QTITEMS_LIBRARY)
#  define QI_EXPORT Q_DECL_EXPORT
#else
#  define QI_EXPORT Q_DECL_IMPORT
#endif

namespace Qi
{

static const quint32 Invalid = -1;
    
enum ChangeReasonFlag
{
    ChangeReasonLinesCount = 0x0001,
    ChangeReasonLinesCountWeak = 0x0002,
    ChangeReasonLineVisibility = 0x0004,
    ChangeReasonLineSize = 0x0008,
    ChangeReasonLineOrder = 0x0010
};

Q_DECLARE_FLAGS(ChangeReason, ChangeReasonFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(ChangeReason)

}

#endif // QI_API_H
