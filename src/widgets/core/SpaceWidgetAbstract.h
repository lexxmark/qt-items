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

#ifndef QI_SPACE_WIDGET_ABSTRACT_H
#define QI_SPACE_WIDGET_ABSTRACT_H

#include "SpaceWidgetCore.h"
#include <QWidget>

namespace Qi
{

class QI_EXPORT SpaceWidgetAbstract: public QWidget, public SpaceWidgetCore
{
    Q_OBJECT
    Q_DISABLE_COPY(SpaceWidgetAbstract)

public:
    virtual ~SpaceWidgetAbstract();

protected:
    explicit SpaceWidgetAbstract(QWidget *parent = nullptr);

    bool event(QEvent* event) override;

    // SpaceWidgetCore implementation
    void ensureVisibleImpl(const ItemID& visibleItem, const CacheSpace *cacheSpace, bool validateItem) override;
};

} // end namespace Qi

#endif // QI_SPACE_WIDGET_ABSTRACT_H
