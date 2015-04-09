#include "SceneWidget.h"
#include "cache/space/CacheSpaceScene.h"
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

void SceneWidget::initScene(const QSharedPointer<SpaceScene>& scene)
{
    Q_ASSERT(!m_scene);

    // initialize main space
    m_scene = scene;
    m_cacheScene = QSharedPointer<CacheSpaceScene>::create(m_scene);

    initSpaceWidgetScrollable(m_cacheScene, m_cacheScene);
}


} // end namespace Qi
