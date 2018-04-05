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

#include "Text.h"
#include <QStyleOptionViewItem>
#include <QLineEdit>

namespace Qi
{

ViewText::ViewText(const SharedPtr<ModelText> &model, ViewDefaultController createDefaultController, Qt::Alignment alignment, Qt::TextElideMode textElideMode)
    : ViewModeled<ModelText>(model),
      m_alignment(alignment),
      m_textElideMode(textElideMode),
      m_margins(2, 0, 2, 0)
{
    if (createDefaultController)
    {
        setController(makeShared<ControllerMouseText>(model));
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

void ViewText::setMargins(const QMargins& margins)
{
    if (m_margins == margins)
        return;

    m_margins = margins;
    emitViewChanged(ChangeReasonViewSize);
}

QSize ViewText::sizeImpl(const GuiContext& ctx, ID id, ViewSizeMode sizeMode) const
{
    return sizeText(theModel()->value(id), ctx, id, sizeMode);
}

void ViewText::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const
{
    drawText(theModel()->value(cache.id), painter, ctx, cache, showTooltip);
}

bool ViewText::textImpl(ID id, QString& txt) const
{
    txt = theModel()->value(id);
    return true;
}

QSize ViewText::sizeText(const QString& text, const GuiContext& ctx, ID /*id*/, ViewSizeMode /*sizeMode*/) const
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
    return QSize(fontMetrics.width(text) + m_margins.left() + m_margins.right(),
                 fontMetrics.height() + m_margins.top() + m_margins.bottom());
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

    QRect rect = cache.cacheView.rect().marginsRemoved(m_margins);
    QString textToDraw = text;
    Qt::TextElideMode elideMode = textElideMode(cache.id);
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

    painter->drawText(rect, alignment(cache.id), textToDraw);
}


ViewTextOrHint::ViewTextOrHint(const SharedPtr<ModelText> &model, ViewDefaultController createDefaultController, Qt::Alignment alignment, Qt::TextElideMode textElideMode)
    : ViewText(model, createDefaultController, alignment, textElideMode)
{
}

QSize ViewTextOrHint::sizeImpl(const GuiContext& ctx, ID id, ViewSizeMode sizeMode) const
{
    if (isItemHint && isItemHint(id, theModel().data()))
    {
        QString hintText = itemHintText ? itemHintText(id, theModel().data()) : QString();
        return sizeText(hintText, ctx, id, sizeMode);
    }
    else
        return ViewText::sizeImpl(ctx, id, sizeMode);
}

void ViewTextOrHint::drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const
{
    if (isItemHint && isItemHint(cache.id, theModel().data()))
    {
        QString hintText = itemHintText ? itemHintText(cache.id, theModel().data()) : QString();
        QPen oldPen = painter->pen();
        painter->setPen(ctx.widget->palette().color(QPalette::Disabled, QPalette::Text));
        drawText(hintText, painter, ctx, cache, showTooltip);
        painter->setPen(oldPen);

        if (showTooltip) *showTooltip = true;
    }
    else
        return ViewText::drawImpl(painter, ctx, cache, showTooltip);
}

bool ViewTextOrHint::tooltipTextImpl(ID id, QString& txt) const
{
    if (isItemHint && isItemHint(id, theModel().data()))
    {
        if (itemHintTooltipText)
            return itemHintTooltipText(id, theModel().data(), txt);
        else
            return false;
    }
    else
        return ViewText::tooltipTextImpl(id, txt);
}

ViewTextFont::ViewTextFont(const SharedPtr<ModelFont> &model)
    : ViewModeled<ModelFont>(model)
{
}

ViewTextFont::ViewTextFont(const QFont& font)
    : ViewModeled<ModelFont>(makeShared<ModelStorageValue<QFont>>(font))
{
}

void ViewTextFont::drawImpl(QPainter* painter, const GuiContext& /*ctx*/, const CacheContext& cache, bool* /*showTooltip*/) const
{
    m_oldFont = painter->font();
    painter->setFont(theModel()->value(cache.id));
}

void ViewTextFont::cleanupDrawImpl(QPainter* painter, const GuiContext& /*ctx*/, const CacheContext& /*cache*/) const
{
    painter->setFont(m_oldFont);
}

ControllerMouseText::ControllerMouseText(SharedPtr<ModelText> model)
    : ControllerMouseInplaceEdit(),
      m_model(std::move(model)),
      m_liveUpdate(false)
{
    Q_ASSERT(m_model);
}

void ControllerMouseText::enableLiveUpdate(bool enable)
{
    m_liveUpdate = enable;
}

bool ControllerMouseText::acceptInplaceEditImpl(ID /*id*/, const CacheSpace& /*cacheSpace*/, const QKeyEvent* keyEvent) const
{
    return !keyEvent || !keyEvent->text().isEmpty();
}

QWidget* ControllerMouseText::createInplaceEditorImpl(ID id, const QRect& rect, QWidget* parent, const QKeyEvent* /*keyEvent*/)
{
    QLineEdit* editor = new QLineEdit(parent);
    editor->setGeometry(rect);
    editor->setText(m_model->value(id));

    connect(editor, &QLineEdit::editingFinished, this, &ControllerMouseText::onEditingFinished);
    if (m_liveUpdate)
        connect(editor, &QLineEdit::textEdited, this, &ControllerMouseText::onTextEdited);

    return editor;
}

void ControllerMouseText::onEditingFinished()
{
    auto editor = inplaceEditor<QLineEdit>();
    if (!editor)
        return;

    m_model->setValue(*activeId(), editor->text());

    // stop edit
    stopInplaceEditor();
}

void ControllerMouseText::onTextEdited(const QString& text)
{
    m_model->setValue(*activeId(), text);
}


} // end namespace Qi
