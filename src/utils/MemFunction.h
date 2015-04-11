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

#ifndef QI_MEM_FUNCTION_H
#define QI_MEM_FUNCTION_H

#include <functional>

namespace Qi
{

template <typename T, typename R>
std::function<R()> memFunction(T* t, R (T::*memFn)())
{
    return std::bind(memFn, t);
}

template <typename T, typename R>
std::function<R()> memFunction(const T* t, R (T::*memFn)() const)
{
    return std::bind(memFn, t);
}

template <typename T, typename R, typename A1>
std::function<R(A1)> memFunction(T* t, R (T::*memFn)(A1))
{
    return std::bind(memFn, t, std::placeholders::_1);
}

template <typename T, typename R, typename A1>
std::function<R(A1)> memFunction(const T* t, R (T::*memFn)(A1) const)
{
    return std::bind(memFn, t, std::placeholders::_1);
}

template <typename T, typename R, typename A1, typename A2>
std::function<R(A1, A2)> memFunction(T* t, R (T::*memFn)(A1, A2))
{
    return std::bind(memFn, t, std::placeholders::_1, std::placeholders::_2);
}

template <typename T, typename R, typename A1, typename A2>
std::function<R(A1, A2)> memFunction(const T* t, R (T::*memFn)(A1, A2) const)
{
    return std::bind(memFn, t, std::placeholders::_1, std::placeholders::_2);
}

template <typename T, typename R, typename A1, typename A2, typename A3>
std::function<R(A1, A2, A3)> memFunction(T* t, R (T::*memFn)(A1, A2, A3))
{
    return std::bind(memFn, t, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

template <typename T, typename R, typename A1, typename A2, typename A3>
std::function<R(A1, A2, A3)> memFunction(const T* t, R (T::*memFn)(A1, A2, A3) const)
{
    return std::bind(memFn, t, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

template <typename T, typename R, typename A1, typename A2, typename A3, typename A4>
std::function<R(A1, A2, A3, A4)> memFunction(T* t, R (T::*memFn)(A1, A2, A3, A4))
{
    return std::bind(memFn, t, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
}

template <typename T, typename R, typename A1, typename A2, typename A3, typename A4>
std::function<R(A1, A2, A3, A4)> memFunction(const T* t, R (T::*memFn)(A1, A2, A3, A4) const)
{
    return std::bind(memFn, t, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
}

} // end namespace Qi

#endif // QI_MEM_FUNCTION_H
