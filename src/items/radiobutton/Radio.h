#ifndef QI_RADIO_H
#define QI_RADIO_H

#include "core/ext/ViewModeled.h"
#include "core/ext/ControllerMousePushable.h"
#include "core/ext/ModelCallback.h"
#include "core/ext/ModelStore.h"

namespace Qi
{

class QI_EXPORT ModelRadio: public ModelComparable
{
    Q_OBJECT
    Q_DISABLE_COPY(ModelRadio)

public:
    bool isRadioItem(const ItemID& item) const { return isRadioItemImpl(item); }
    bool setRadioItem(const ItemID& item) { return setRadioItemImpl(item); }

protected:
    ModelRadio() {}

    int compareImpl(const ItemID& left, const ItemID& right) const override;
    bool isAscendingDefaultImpl(const ItemID& /*item*/) const override { return false; }

    virtual bool isRadioItemImpl(const ItemID& item) const = 0;
    virtual bool setRadioItemImpl(const ItemID& item) = 0;
};

class QI_EXPORT ModelRadioCallback: public ModelRadio
{
    Q_OBJECT
    Q_DISABLE_COPY(ModelRadioCallback)

public:
    ModelRadioCallback() {}

    std::function<bool(const ItemID&)> isRadioItem;
    std::function<bool(const ItemID&)> setRadioItem;

protected:
    bool isRadioItemImpl(const ItemID& item) const override;
    bool setRadioItemImpl(const ItemID& item) override;
};

class QI_EXPORT ModelRadioStorage: public ModelRadio
{
    Q_OBJECT
    Q_DISABLE_COPY(ModelRadioStorage)

public:
    ModelRadioStorage(const ItemID& radioItem = ItemID(0, 0));

    const ItemID& radioItem() const { return m_radioItem; }

protected:
    bool isRadioItemImpl(const ItemID& item) const override;
    bool setRadioItemImpl(const ItemID& item) override;

private:
    ItemID m_radioItem;
};

class QI_EXPORT ViewRadio: public ViewModeled<ModelRadio>
{
    Q_OBJECT
    Q_DISABLE_COPY(ViewRadio)

public:
    ViewRadio(const QSharedPointer<ModelRadio>& model, bool useDefaultController = true);

protected:
    QSize sizeImpl(const GuiContext& ctx, const ItemID& item, ViewSizeMode sizeMode) const override;
    void drawImpl(QPainter* painter, const GuiContext& ctx, const CacheContext& cache, bool* showTooltip) const override;

private:
    QStyle::State styleState(const ItemID& item) const;

    PushableTracker m_pushableTracker;
};

QI_EXPORT QSharedPointer<ControllerMousePushable> createControllerMouseRadio(const QSharedPointer<ModelRadio>& model);

} // end namespace Qi

#endif // QI_VIEW_RADIO_H
