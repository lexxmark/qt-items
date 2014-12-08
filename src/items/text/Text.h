#ifndef QI_TEXT_H
#define QI_TEXT_H

#include "core/ext/ViewModeled.h"
#include "core/ext/ControllerMousePushable.h"
#include "core/ext/ModelCallback.h"
#include "core/ext/ModelStore.h"
#include <QString>

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
    ViewText(const QSharedPointer<ModelText>& model, bool useDefaultController = false, Qt::Alignment alignment = Qt::Alignment(Qt::AlignLeft | Qt::AlignVCenter), Qt::TextElideMode textElideMode = Qt::ElideNone);

    Qt::Alignment alignment(const ItemID& item) const { return alignmentImpl(item); }
    void setAlignment(Qt::Alignment alignment);

    Qt::TextElideMode textElideMode(const ItemID& item) const { return textElideModeImpl(item); }
    void setTextElideMode(Qt::TextElideMode textElideMode);

protected:
    virtual Qt::Alignment alignmentImpl(const ItemID& /*item*/) const { return m_alignment; }
    virtual Qt::TextElideMode textElideModeImpl(const ItemID& /*item*/) const { return m_textElideMode; }

    QSize sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const override;
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;
    bool textImpl(const ItemID& item, QString& txt) const override;

private:
    Qt::Alignment m_alignment;
    Qt::TextElideMode m_textElideMode;
};

} // end namespace Qi

#endif // QI_TEXT_H
