#ifndef QI_LAYOUT_H
#define QI_LAYOUT_H

#include <QObject>
#include "View.h"

namespace Qi
{

class QI_EXPORT Layout: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Layout)

public:
    Layout(QObject* parent = nullptr);
    virtual ~Layout();
    
    void doLayout(const View& view, const CellID& cell, QRect& availableRect) const { doLayoutImpl(view, cell, availableRect); }
    
Q_SIGNALS:
    void layoutChanged(const Layout&);

protected:
    virtual void doLayoutImpl(const View& view, const CellID& cell, QRect& availableRect) const = 0;
};

} // end namespace Qi

#endif // QI_LAYOUT_H
