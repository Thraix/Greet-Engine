#pragma once


#include "LineBatchRenderer.h"

#include <event/Event.h>
#include <ecs/ECSScene.h>
#include <graphics/renderers/BatchRenderer.h>
#include <graphics/RenderCommand.h>
#include <ecs/components/Camera2DComponent.h>

class EntityManager;

class Entity2DManager
{

  struct Grid
  {
    Greet::Vec2f boundsMin;
    Greet::Vec2f boundsMax;
    int count;
    Greet::Vec2f center;
    float size;

    Grid(Greet::Camera2DComponent& camera)
    {
      const float MIN_GRID_SIZE = 20;
      Greet::Mat3 invPVMatrix = camera.GetInversePVMatrix();
      boundsMin = invPVMatrix * Greet::Vec2f{-1, -1};
      boundsMax = invPVMatrix * Greet::Vec2f{1, 1};
      Greet::Vec2f worldCoordSize = (boundsMin - boundsMax).Abs();
      Greet::Vec2f worldCoordCenter = invPVMatrix * Greet::Vec2f{0, 0};

      Greet::Vec2f viewportSize = Greet::Vec2f{Greet::RenderCommand::GetViewportWidth(), Greet::RenderCommand::GetViewportHeight()};
      float gridCountF = viewportSize.x / MIN_GRID_SIZE;
      size = pow(10, ceil(log10(worldCoordSize.x / gridCountF)));
      center = Greet::Vec2f{ceil(worldCoordCenter.x / size) * size, ceil(worldCoordCenter.y / size) * size};
      count = ceil(gridCountF);
    }
  };

  struct EntityHolder
  {
    bool holding = false;
    bool movable = false;
    Greet::Vec2f pressPos;
    Greet::Vec2f entityPressPos;
  };

  EntityManager* entityManager;
  Greet::ECSScene* scene;
  EntityHolder entityHolder;
  Greet::Ref<Greet::LineBatchRenderer> lineBatchRenderer;

  public:
    Entity2DManager(EntityManager* entityManager, Greet::ECSScene* scene);

    void OnEvent(Greet::Event& event);
    void UpdateEntityPosition(Greet::Entity entity, const Greet::Vec2f& mousePos);
    void RenderPre() const;
    void RenderPost() const;
};
