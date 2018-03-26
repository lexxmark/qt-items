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

#include "Numeric.h"
#include "space/grid/GridID.h"

namespace Qi
{

ModelRowNumber::ModelRowNumber(bool ascendingDefault)
{
    m_ascendingDefault = ascendingDefault;
}

int ModelRowNumber::valueImpl(ID id) const
{
    return row(id);
}

bool ModelRowNumber::setValueImpl(ID /*id*/, ValueType_t /*value*/)
{
    // not applicable
    return false;
}

ModelColumnNumber::ModelColumnNumber(bool ascendingDefault)
{
    m_ascendingDefault = ascendingDefault;
}

int ModelColumnNumber::valueImpl(ID id) const
{
    return column(id);
}

bool ModelColumnNumber::setValueImpl(ID /*id*/, ValueType_t /*value*/)
{
    // not applicable
    return false;
}

} // end namespace Qi
