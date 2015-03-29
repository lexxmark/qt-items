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
