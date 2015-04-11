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
