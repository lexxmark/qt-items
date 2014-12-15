#include "Text.h"
#include <QStyleOptionViewItem>

namespace Qi
{

ViewText::ViewText(const QSharedPointer<ModelText>& model, bool useDefaultController, Qt::Alignment alignment, Qt::TextElideMode textElideMode)
    : ViewModeled<ModelText>(model),
      m_alignment(alignment),
      m_textElideMode(textElideMode)
{
    if (useDefaultController)
    {
 //       setController(QSharedPointer<ControllerMouseText>::create(model));
    }
}

void ViewText::setAlignment(Qt::Alignment alignment)
{
    if (m_alignment == alignment)
        return;

    m_alignment = alignment;
    emit viewChanged(this, ChangeReasonViewContent);
}

void ViewText::setTextElideMode(Qt::TextElideMode textElideMode)
{
    if (m_textElideMode == textElideMode)
        return;

    m_textElideMode = textElideMode;
    emit viewChanged(this, ChangeReasonViewContent);
}

QSize ViewText::sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode /*sizeMode*/) const
{
    /*
    QStyleOptionViewItem option;
    option.initFrom(ctx.widget);
    option.text = theModel()->value(item);
    option.displayAlignment = alignment(item);
    option.textElideMode = textElideMode(item);
    option.widget = ctx.widget;

    return ctx.widget->style()->sizeFromContents(QStyle::CT_ItemViewItem, &option, QSize(0, 0), ctx.widget) + QSize(5, 5);
    */
    QString text = theModel()->value(item);
    QFontMetrics fontMetrics = ctx.widget->fontMetrics();
    return QSize(fontMetrics.width(text), fontMetrics.height());
}

void ViewText::drawImpl(QPainter* painter, const GuiContext& /*ctx*/, const CacheContext& cache, bool* showTooltip) const
{
    /*
     * cannot use this stuff because textElideMode is not working
    QStyleOptionViewItem option;
    option.initFrom(ctx.widget);
    option.rect = cache.cacheView.rect();
    option.text = theModel()->value(cache.item);
    option.displayAlignment = alignment(cache.item);
    option.textElideMode = textElideMode(cache.item);
    option.widget = ctx.widget;

    ctx.widget->style()->drawControl(QStyle::CE_ItemViewItem, &option, painter, ctx.widget);
    */

    QRect rect = cache.cacheView.rect();
    QString text = theModel()->value(cache.item);
    Qt::TextElideMode elideMode = textElideMode(cache.item);
    if (elideMode != Qt::ElideNone)
    {
        QString elidedText = painter->fontMetrics().elidedText(text, elideMode, rect.width());
        if (showTooltip)
            *showTooltip = (elidedText != text);

        text = elidedText;
    }
    else
    {
        if (showTooltip)
            *showTooltip = (painter->fontMetrics().width(text) > rect.width());
    }

    painter->drawText(rect, alignment(cache.item), text);
}

bool ViewText::textImpl(const ItemID& item, QString& txt) const
{
    txt = theModel()->value(item);
    return true;
}

} // end namespace Qi
