#include "Filter.h"
#include "core/Model.h"

namespace Qi
{

ItemsFilter::ItemsFilter(const QSharedPointer<Model>& modelToFilter)
    : m_modelToFilter(modelToFilter)
{
    if (!m_modelToFilter.isNull())
        connect(m_modelToFilter.data(), &Model::modelChanged, this, &ItemsFilter::onModelToFilterChanged);
}

ItemsFilter::~ItemsFilter()
{
    if (!m_modelToFilter.isNull())
        disconnect(m_modelToFilter.data(), &Model::modelChanged, this, &ItemsFilter::onModelToFilterChanged);
}

void ItemsFilter::onModelToFilterChanged(const Model*)
{
    emit filterChanged(this);
}

} // end namespace Qi
