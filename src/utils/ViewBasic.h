#ifndef QI_VIEW_BASIC_H
#define QI_VIEW_BASIC_H

#include "View.h"

namespace Qi
{

class QI_EXPORT ViewText: public View
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewText)

public:
    ViewText();

protected:
    void drawImpl(QPainter* painter, const QWidget* widget, const CellID& cell, const QRect& rect) const override;
    QSize sizeHintImpl(const QWidget* widget, const CellID& cell) const override;
    bool textImpl(const CellID& cell, QString& text) const override;
};

class QI_EXPORT ViewCheck: public View
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewCheck)

public:
    ViewCheck();

protected:
    void drawImpl(QPainter* painter, const QWidget* widget, const CellID& cell, const QRect& rect) const override;
    QSize sizeHintImpl(const QWidget* widget, const CellID& cell) const override;
    bool textImpl(const CellID& cell, QString& text) const override;
};

} // end namespace Qi

#endif // QI_VIEW_BASIC_H
