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
    void onModelChanged(const Model*) { emitViewChanged(ChangeReasonViewContent); }

private:
    QSharedPointer<Model_t> m_model;
};

} // end namespace Qi

#endif // QI_VIEW_MODELED_H
