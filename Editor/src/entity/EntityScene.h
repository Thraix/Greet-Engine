#pragma once

#include <ecs/ECSScene.h>

namespace Greet
{
  class EntityRendering;
  class EntityManager;
  class Entity2DManager;

  enum class ActiveScene
  {
    _2D, _3D, Runtime
  };

  class EntityScene : public ECSScene
  {
    EntityManager* entityManager;
    Ref<Entity2DManager> entity2DManager;

    Ref<EntityRendering> entityRendering;
    ActiveScene activeScene = ActiveScene::_2D;


    public:
      EntityScene(EntityManager* entityManager);

      void OnEvent(Event& event) override;
      void OnEvent3D(Event& event);

      void Render() const override;
      void Render2D() const override;
      void Render3D(const Camera3DComponent& cam, const Environment3DComponent& env) const override;

      void SelectEntity(Entity entity);
      void UpdateSelectedTransform3D();

      Entity GetCamera2DEntity() const override;
      Entity GetCamera3DEntity() const override;
      Entity GetEnvironment2DEntity() const override;
      Entity GetEnvironment3DEntity() const override;

    private:
      Entity GetNearestRaycastedEntity(Camera3DComponent& cameraComponent, const Vec2f& pos, float farDistance = 100);
  };
}
