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

#ifndef QI_ITEM_WIDGET_H
#define QI_ITEM_WIDGET_H

#include "space/SpaceItem.h"
#include "core/SpaceWidgetAbstract.h"

namespace Qi
{

class QI_EXPORT ItemWidget: public SpaceWidgetAbstract
{
    Q_OBJECT
    Q_DISABLE_COPY(ItemWidget)

public:
    explicit ItemWidget(QWidget *parent = nullptr);
    virtual ~ItemWidget();

    const SpaceItem& spaceItem() const { return *m_space; }
    SpaceItem& spaceItem() { return *m_space; }

    // there are two ways to synchronize space size:
    // 1. sync with widget size (minimumSizeHint will work by default)
    // 2. sync with item content (minimumSizeHint will return desired item size)
    void syncSpaceSizeWithContent(bool enable);

protected:
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    bool event(QEvent* e) override;

private:
    void onSpaceChanged(const Space* space, ChangeReason reason);

    QSharedPointer<SpaceItem> m_space;
    bool m_syncSpaceSizeWithContent;
};

} // end namespace Qi

#endif // QI_ITEM_WIDGET_H
