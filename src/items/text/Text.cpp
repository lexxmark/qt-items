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
    emitViewChanged(ChangeReasonViewContent);
}

void ViewText::setTextElideMode(Qt::TextElideMode textElideMode)
{
    if (m_textElideMode == textElideMode)
        return;

    m_textElideMode = textElideMode;
    emitViewChanged(ChangeReasonViewContent);
}

QSize ViewText::sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const
{
    return sizeText(theModel()->value(item), ctx, item, sizeMode);
}

void ViewText::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const
{
    drawText(theModel()->value(cache.item), painter, ctx, cache, showTooltip);
}

bool ViewText::textImpl(const ItemID& item, QString& txt) const
{
    txt = theModel()->value(item);
    return true;
}

QSize ViewText::sizeText(const QString& text, const GuiContext& ctx, const ItemID& /*item*/, ViewSizeMode /*sizeMode*/) const
{
    /*
    QStyleOptionViewItem option;
    option.initFrom(ctx.widget);
    option.text = text;
    option.displayAlignment = alignment(item);
    option.textElideMode = textElideMode(item);
    option.widget = ctx.widget;

    return ctx.widget->style()->sizeFromContents(QStyle::CT_ItemViewItem, &option, QSize(0, 0), ctx.widget) + QSize(5, 5);
    */
    QFontMetrics fontMetrics = ctx.widget->fontMetrics();
    return QSize(fontMetrics.width(text), fontMetrics.height());
}

void ViewText::drawText(const QString& text, QPainter* painter, const GuiContext& /*ctx*/, const CacheContext& cache, bool* showTooltip) const
{
    /*
     * cannot use this stuff because textElideMode is not working
    QStyleOptionViewItem option;
    option.initFrom(ctx.widget);
    option.rect = cache.cacheView.rect();
    option.text = text;
    option.displayAlignment = alignment(cache.item);
    option.textElideMode = textElideMode(cache.item);
    option.widget = ctx.widget;

    ctx.widget->style()->drawControl(QStyle::CE_ItemViewItem, &option, painter, ctx.widget);
    */

    QRect rect = cache.cacheView.rect();
    QString textToDraw = text;
    Qt::TextElideMode elideMode = textElideMode(cache.item);
    if (elideMode != Qt::ElideNone)
    {
        QString elidedText = painter->fontMetrics().elidedText(textToDraw, elideMode, rect.width());
        if (showTooltip)
            *showTooltip = (elidedText != textToDraw);

        textToDraw = elidedText;
    }
    else
    {
        if (showTooltip)
            *showTooltip = (painter->fontMetrics().width(text) > rect.width());
    }

    painter->drawText(rect, alignment(cache.item), textToDraw);
}


ViewTextOrHint::ViewTextOrHint(const QSharedPointer<ModelText>& model, bool useDefaultController, Qt::Alignment alignment, Qt::TextElideMode textElideMode)
    : ViewText(model, useDefaultController, alignment, textElideMode)
{
}

QSize ViewTextOrHint::sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const
{
    if (isItemHint && isItemHint(item, theModel().data()))
    {
        QString hintText = itemHintText ? itemHintText(item, theModel().data()) : QString();
        return sizeText(hintText, ctx, item, sizeMode);
    }
    else
        return ViewText::sizeImpl(ctx, item, sizeMode);
}

void ViewTextOrHint::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const
{
    if (isItemHint && isItemHint(cache.item, theModel().data()))
    {
        QString hintText = itemHintText ? itemHintText(cache.item, theModel().data()) : QString();
        QPen oldPen = painter->pen();
        painter->setPen(ctx.widget->palette().color(QPalette::Disabled, QPalette::Text));
        drawText(hintText, painter, ctx, cache, showTooltip);
        painter->setPen(oldPen);
    }
    else
        return ViewText::drawImpl(painter, ctx, cache, showTooltip);
}

bool ViewTextOrHint::tooltipTextImpl(const ItemID& item, QString& txt) const
{
    if (isItemHint && isItemHint(item, theModel().data()))
    {
        if (itemHintTooltipText)
            return itemHintTooltipText(item, theModel().data(), txt);
        else
            return false;
    }
    else
        return ViewText::tooltipTextImpl(item, txt);
}


} // end namespace Qi
