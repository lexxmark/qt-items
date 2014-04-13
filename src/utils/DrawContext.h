#ifndef QI_DRAWCONTEXT_H
#define QI_DRAWCONTEXT_H

#include "../QiAPI.h"
#include <QPainter>
#include <QStyle>
#include <QWidget>

class QI_EXPORT DrawContext
{
public:
    QPainter* painter;
    const QStyle* style;
    const QWidget* widget;
};

#endif // QI_DRAWCONTEXT_H
