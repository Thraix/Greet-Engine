#pragma once

#include <ecs/ECSScene.h>

class EntityRendering;
class EntityManager;

enum class ActiveScene
{
  _2D, _3D
};

class EntityScene : public Greet::ECSScene
{
  EntityManager* entityManager;

  Greet::Ref<EntityRendering> entityRendering;
  ActiveScene activeScene = ActiveScene::_2D;


  public:
    EntityScene(EntityManager* entityManager);

    void OnEvent(Greet::Event& event) override;
    void OnEvent2D(Greet::Event& event);
    void OnEvent3D(Greet::Event& event);

    void Render() const override;
    void Render3D(const Greet::Camera3DComponent& cam, const Greet::Environment3DComponent& env) const override;

    void SelectEntity(Greet::Entity entity);
    void UpdateSelectedTransform2D();
    void UpdateSelectedTransform3D();

    Greet::Entity GetCamera2DEntity() const override;
    Greet::Entity GetCamera3DEntity() const override;
    Greet::Entity GetEnvironment2DEntity() const override;
    Greet::Entity GetEnvironment3DEntity() const override;

  private:
    Greet::Entity GetNearestRaycastedEntity(Greet::Camera3DComponent& cameraComponent, const Greet::Vec2f& pos, float farDistance = 100);
};
