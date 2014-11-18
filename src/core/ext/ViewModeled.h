#ifndef QI_VIEW_MODELED_H
#define QI_VIEW_MODELED_H

#include "core/View.h"
#include "core/Model.h"

namespace Qi
{

template <typename Model_t> class ViewModeled: public View
{
public:
    ViewModeled(const QSharedPointer<Model_t>& model)
        : m_model(model)
    {
        Q_ASSERT(m_model);
        connect(m_model.data(), &Model::modelChanged, this, &ViewModeled::onModelChanged);
    }

    ~ViewModeled()
    {
        disconnect(m_model.data(), &Model::modelChanged, this, &ViewModeled::onModelChanged);
    }

    const QSharedPointer<Model_t>& theModel() const { return m_model; }

protected:
    Model* modelImpl() override { return m_model.data(); }

private slots:
    void onModelChanged(const Model*) { emit viewChanged(this, ChangeReasonViewContent); }

private:
    QSharedPointer<Model_t> m_model;
};

} // end namespace Qi

#endif // QI_VIEW_MODELED_H
