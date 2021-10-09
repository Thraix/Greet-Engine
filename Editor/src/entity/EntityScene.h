#pragma once

#include <ecs/ECSScene.h>

class EntityRendering;
class EntityManager;

class EntityScene : public Greet::ECSScene
{
  EntityManager* entityManager;

  Greet::Ref<EntityRendering> entityRendering;

  public:
    EntityScene(EntityManager* entityManager);

    void OnEvent(Greet::Event& event) override;
    void Render3D(const Greet::Camera3DComponent& cam, const Greet::Environment3DComponent& env) const override;

    void SelectEntity(Greet::Entity entity);
    void UpdateSelectedTransform3D();

    Greet::Entity GetCameraEntity() const;

  private:
    Greet::Entity GetNearestRaycastedEntity(Greet::Camera3DComponent& cameraComponent, const Greet::Vec2f& pos, float farDistance = 100);
};
