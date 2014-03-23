#ifndef QI_VIEW_H
#define QI_VIEW_H

#include <functional>
#include <QObject>
#include <QPainter>
#include "CellID.h"

namespace Qi
{

class QI_EXPORT View: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(View)

public:
    View(QObject* parent = nullptr);
    virtual ~View();
    
    void draw(QPainter& painter, const CellID& cell, const QRect& rect) const;
    QSize sizeHint(QPainter& painter, const CellID& cell) const;
    bool text(const CellID& cell, QString& text) const;
    bool tooltipText(const CellID& cell, QString& text) const;
    
    std::function<bool(const CellID& cell, QString& text)> tooltipTextCallback;
    
Q_SIGNALS:
    void viewChanged(const View&);

protected:
    virtual void drawImpl(QPainter& painter, const CellID& cell, const QRect& rect) const;
    virtual QSize sizeHintImpl(QPainter& painter, const CellID& cell) const;
    virtual bool textImpl(const CellID& cell, QString& text) const;
};

} // end namespace Qi

#endif // QI_VIEW_H
