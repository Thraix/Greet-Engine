#pragma once

#include <graphics/shaders/Shader.h>

#include "EntityRendering.h"

class EntityManager;

class EditorScene : public Greet::ECSScene
{
  EntityManager* entityManager;

  Greet::Ref<EntityRendering> entityRendering;

  public:
    EditorScene(EntityManager* entityManager);

    void OnEvent(Greet::Event& event) override;
    void Render3D(const Greet::Camera3DComponent& cam, const Greet::Environment3DComponent& env) const override;

    void SelectEntity(Greet::Entity entity);

    Greet::Entity GetCameraEntity() const;

  private:
    Greet::Entity GetNearestRaycastedEntity(Greet::Camera3DComponent& cameraComponent, const Greet::Vec2f& pos, float farDistance = 100);
};
