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

#ifndef QI_TEXT_H
#define QI_TEXT_H

#include "core/ext/ViewModeled.h"
#include "core/ext/ControllerMouseInplaceEdit.h"
#include "core/ext/ModelCallback.h"
#include "core/ext/ModelStore.h"
#include <QString>
#include <QMargins>

namespace Qi
{

typedef ModelTyped<QString> ModelText;
typedef ModelCallback<QString> ModelTextCallback;
typedef ModelStorageValue<QString> ModelTextValue;

class QI_EXPORT ViewText: public ViewModeled<ModelText>
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewText)

public:
    ViewText(const SharedPtr<ModelText>& model, ViewDefaultController createDefaultController = ViewDefaultControllerNone, Qt::Alignment alignment = Qt::Alignment(Qt::AlignLeft | Qt::AlignVCenter), Qt::TextElideMode textElideMode = Qt::ElideNone);

    Qt::Alignment alignment(ID id) const { return alignmentImpl(id); }
    void setAlignment(Qt::Alignment alignment);

    Qt::TextElideMode textElideMode(ID id) const { return textElideModeImpl(id); }
    void setTextElideMode(Qt::TextElideMode textElideMode);

    const QMargins& margins() const { return m_margins; }
    void setMargins(const QMargins& margins);

protected:
    virtual Qt::Alignment alignmentImpl(ID /*id*/) const { return m_alignment; }
    virtual Qt::TextElideMode textElideModeImpl(ID /*id*/) const { return m_textElideMode; }

    QSize sizeImpl(const GuiContext& ctx, ID id, ViewSizeMode sizeMode) const override;
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;
    bool textImpl(ID id, QString& txt) const override;

    QSize sizeText(const QString& text, const GuiContext& ctx, ID id, ViewSizeMode sizeMode) const;
    void drawText(const QString& text, QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const;

private:
    Qt::Alignment m_alignment;
    Qt::TextElideMode m_textElideMode;
    QMargins m_margins;
};

class QI_EXPORT ViewTextOrHint: public ViewText
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewTextOrHint)

public:
    ViewTextOrHint(const SharedPtr<ModelText>& model, ViewDefaultController createDefaultController = ViewDefaultControllerNone, Qt::Alignment alignment = Qt::Alignment(Qt::AlignLeft | Qt::AlignVCenter), Qt::TextElideMode textElideMode = Qt::ElideNone);

    std::function<bool(ID, const ModelText*)> isItemHint;
    std::function<QString(ID, const ModelText*)> itemHintText;
    std::function<bool(ID, const ModelText*, QString&)> itemHintTooltipText;

protected:
    QSize sizeImpl(const GuiContext& ctx, ID id, ViewSizeMode sizeMode) const override;
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;
    bool tooltipTextImpl(ID id, QString& txt) const override;
};

typedef ModelTyped<QFont> ModelFont;

class QI_EXPORT ViewTextFont: public ViewModeled<ModelFont>
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewTextFont)

public:
    explicit ViewTextFont(const SharedPtr<ModelFont>& model);
    explicit ViewTextFont(const QFont& font);

protected:
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;
    void cleanupDrawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache) const override;

private:
    mutable QFont m_oldFont;
};

class QI_EXPORT ControllerMouseText: public ControllerMouseInplaceEdit
{
public:
    ControllerMouseText(SharedPtr<ModelText> model);

    void enableLiveUpdate(bool enable = true);

protected:
    bool acceptInplaceEditImpl(ID /*id*/, const CacheSpace& /*cacheSpace*/, const QKeyEvent* /*keyEvent*/) const override;
    QWidget* createInplaceEditorImpl(ID id, const QRect& rect, QWidget* parent, const QKeyEvent* keyEvent) override;

private:
    void onEditingFinished();
    void onTextEdited(const QString& text);

    SharedPtr<ModelText> m_model;
    bool m_liveUpdate;
};

} // end namespace Qi

#endif // QI_TEXT_H
