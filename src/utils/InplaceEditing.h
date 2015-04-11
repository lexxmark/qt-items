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

#ifndef QI_INPLACE_EDITING_H
#define QI_INPLACE_EDITING_H

#include "QiAPI.h"
#include <QWidget>

namespace Qi
{

class QI_EXPORT InplaceEdit: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(InplaceEdit)

public:
    bool start(QWidget *editor);
    QWidget *current() { return m_inplaceEditor; }
    bool stop();

private:
    InplaceEdit();

    void onEditorDestroyed(QObject* object);
    bool eventFilter(QObject* watched, QEvent* event) override;

    QWidget* m_inplaceEditor;

    friend QI_EXPORT InplaceEdit& inplaceEdit();
};

QI_EXPORT InplaceEdit& inplaceEdit();

} // end namespace Qi

#endif // QI_INPLACE_EDITING_H
