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

#ifndef AUTO_VALUE_H
#define AUTO_VALUE_H

template <typename T>
struct auto_value
{
    auto_value(T& value, T newValue)
        : m_value(value),
          m_oldValue(value)
    {
        m_value = newValue;
    }

    ~auto_value()
    {
        m_value = m_oldValue;
    }

private:
    T& m_value;
    T m_oldValue;
};

#endif // AUTO_VALUE_H
