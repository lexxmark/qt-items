#ifndef QI_SCENE_WIDGET_H
#define QI_SCENE_WIDGET_H

#include "QiAPI.h"
#include "space/SpaceScene.h"
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

    const QSharedPointer<SpaceScene>& scene() const { return m_scene; }
    void initScene(const QSharedPointer<SpaceScene>& scene);

    const QSharedPointer<CacheSpaceScene>& cacheScene() const { return m_cacheScene;}

private:
    QSharedPointer<SpaceScene> m_scene;
    QSharedPointer<CacheSpaceScene> m_cacheScene;
};

} // end namespace Qi

#endif // QI_SCENE_WIDGET_H
