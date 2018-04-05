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

#ifndef QI_SCENE_WIDGET_H
#define QI_SCENE_WIDGET_H

#include "QiAPI.h"
#include "space/scene/SpaceScene.h"
#include "core/SpaceWidgetScrollAbstract.h"

namespace Qi
{

class CacheSpaceScene;

class QI_EXPORT SceneWidget: public SpaceWidgetScrollAbstract
{
    Q_OBJECT
    Q_DISABLE_COPY(SceneWidget)

public:
    explicit SceneWidget(QWidget *parent = nullptr);
    virtual ~SceneWidget();

    const SharedPtr<SpaceScene>& scene() const { return m_scene; }
    void initScene(SharedPtr<SpaceScene> scene);

    const SharedPtr<CacheSpaceScene>& cacheScene() const { return m_cacheScene;}

private:
    SharedPtr<SpaceScene> m_scene;
    SharedPtr<CacheSpaceScene> m_cacheScene;
};

} // end namespace Qi

#endif // QI_SCENE_WIDGET_H
