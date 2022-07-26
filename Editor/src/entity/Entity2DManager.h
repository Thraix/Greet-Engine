#pragma once


#include <event/Event.h>
#include <graphics/RenderCommand.h>
#include <ecs/components/Camera2DComponent.h>
#include <ecs/Entity.h>

namespace Greet
{
  class BatchRenderer;
  class ECSScene;

  class EntityManager;
  class LineBatchRenderer;

  class Entity2DManager
  {
    struct Grid
    {
      Vec2f boundsMin;
      Vec2f boundsMax;
      int count;
      Vec2f center;
      float size;

      Grid(Camera2DComponent& camera)
      {
        const float MIN_GRID_SIZE = 20;
        Mat3 invPVMatrix = camera.GetInversePVMatrix();
        boundsMin = invPVMatrix * Vec2f{-1, -1};
        boundsMax = invPVMatrix * Vec2f{1, 1};
        Vec2f worldCoordSize = (boundsMin - boundsMax).Abs();
        Vec2f worldCoordCenter = invPVMatrix * Vec2f{0, 0};

        Vec2f viewportSize = Vec2f{RenderCommand::GetViewportWidth(), RenderCommand::GetViewportHeight()};
        float gridCountF = viewportSize.x / MIN_GRID_SIZE;
        size = pow(10, ceil(log10(worldCoordSize.x / gridCountF)));
        center = Vec2f{ceil(worldCoordCenter.x / size) * size, ceil(worldCoordCenter.y / size) * size};
        count = ceil(gridCountF);
      }
    };

    struct EntityHolder
    {
      bool holding = false;
      bool movable = false;
      Vec2f pressPos;
      Vec2f entityPressPos;
    };

    private:
      EntityManager* entityManager;
      ECSScene* scene;
      EntityHolder entityHolder;
      Ref<LineBatchRenderer> lineBatchRenderer;

    public:
      Entity2DManager(EntityManager* entityManager, ECSScene* scene);

      void OnEvent(Event& event);
      void UpdateEntityPosition(Entity entity, const Vec2f& mousePos);
      void RenderPre() const;
      void RenderPost() const;
  };
}
