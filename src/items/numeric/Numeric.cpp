#include "Numeric.h"

namespace Qi
{

ModelRowNumber::ModelRowNumber(bool ascendingDefault)
{
    m_ascendingDefault = ascendingDefault;
}

int ModelRowNumber::valueImpl(const ItemID& item) const
{
    return item.row;
}

bool ModelRowNumber::setValueImpl(const ItemID& /*item*/, ValueType_t /*value*/)
{
    // not applicable
    return false;
}

ModelColumnNumber::ModelColumnNumber(bool ascendingDefault)
{
    m_ascendingDefault = ascendingDefault;
}

int ModelColumnNumber::valueImpl(const ItemID& item) const
{
    return item.column;
}

bool ModelColumnNumber::setValueImpl(const ItemID& /*item*/, ValueType_t /*value*/)
{
    // not applicable
    return false;
}

} // end namespace Qi
