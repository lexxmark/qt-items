#ifndef QI_CALL_LATER_H
#define QI_CALL_LATER_H

#include "QiAPI.h"
//#include <QWidget>
#include <functional>

namespace Qi
{

QI_EXPORT void callLater(QObject* owner, const std::function<void()>& callback);

} // end namespace Qi

#endif // QI_CALL_LATER_H
