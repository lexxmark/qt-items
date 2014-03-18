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
}

#endif // QI_API_H
