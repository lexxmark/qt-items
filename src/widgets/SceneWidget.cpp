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

#include "SceneWidget.h"
#include "space/scene/CacheSpaceScene.h"
#include "cache/CacheItem.h"

namespace Qi
{

SceneWidget::SceneWidget(QWidget* parent)
    : SpaceWidgetScrollAbstract(parent)
{
}

SceneWidget::~SceneWidget()
{
}

void SceneWidget::initScene(SharedPtr<SpaceScene> scene)
{
    Q_ASSERT(!m_scene);

    // initialize main space
    m_scene = std::move(scene);
    m_cacheScene = makeShared<CacheSpaceScene>(m_scene);

    initSpaceWidgetScrollable(m_cacheScene, m_cacheScene);
}


} // end namespace Qi
