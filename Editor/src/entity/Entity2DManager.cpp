#include "Entity2DManager.h"

#include "EntityManager.h"
#include "LineBatchRenderer.h"

#include <ecs/ECSScene.h>
#include <ecs/components/Camera2DComponent.h>
#include <ecs/components/ColorComponent.h>
#include <ecs/components/Environment2DComponent.h>
#include <ecs/components/SpriteComponent.h>
#include <ecs/components/Transform2DComponent.h>
#include <event/KeyEvent.h>
#include <event/MouseEvent.h>
#include <graphics/renderers/BatchRenderer.h>
#include <graphics/shaders/ShaderFactory.h>
#include <graphics/textures/TextureManager.h>
#include <input/Input.h>

namespace Greet
{
  Entity2DManager::Entity2DManager(EntityManager* entityManager, ECSScene* scene)
    : entityManager{entityManager}, scene{scene},
      lineBatchRenderer{NewRef<LineBatchRenderer>()},
      camera2DController{entityManager->GetECS().get()}
  {}

  void Entity2DManager::OnEvent(Event& event)
  {
    camera2DController.OnEvent(event);
    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      if(e.GetButton() == GREET_MOUSE_LEFT)
      {
        Camera2DComponent& cam = scene->GetCamera2DEntity().GetComponent<Camera2DComponent>();
        Vec2f mousePos = cam.GetInversePVMatrix() * e.GetPosition();
        Entity pressedEntity{scene->GetManager()};
        scene->GetManager()->Each<Transform2DComponent>([&](EntityID entity, Transform2DComponent& transform)
        {
          if(scene->GetManager()->HasAnyComponent<SpriteComponent, ColorComponent>(entity))
          {
            Vec2f mousePosRelativeToEntity = transform.GetInverseTransform() * mousePos;
            if(mousePosRelativeToEntity > -0.5 && mousePosRelativeToEntity < 0.5)
            {
              pressedEntity = entity;
            }
          }
        });
        entityHolder.holding = false;
        entityHolder.movable = false;
        if(pressedEntity != entityManager->GetSelectedEntity())
        {
          entityManager->SelectEntity(pressedEntity);
        }
        if(pressedEntity)
        {
          entityHolder.holding = true;
          entityHolder.movable = false;
          entityHolder.entityPressPos = pressedEntity.GetComponent<Transform2DComponent>().position;
          entityHolder.pressPos = e.GetPosition();
        }
      }
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_RELEASE))
    {
      entityHolder.holding = false;
      entityHolder.movable = false;
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      if(entityHolder.holding)
      {
        MouseMoveEvent& e = static_cast<MouseMoveEvent&>(event);
        Camera2DComponent& cam = scene->GetCamera2DEntity().GetComponent<Camera2DComponent>();
        if(!entityHolder.movable)
        {
          Mat3 invViewportMatrix = ~Mat3::OrthographicViewport();
          Vec2f oldMousePos = invViewportMatrix * entityHolder.pressPos;
          Vec2f newMousePos = invViewportMatrix * e.GetPosition();
          if((newMousePos - oldMousePos).LengthSQ() > 10 * 10)
          {
            entityHolder.movable = true;
          }
        }
        else
        {
          UpdateEntityPosition(entityManager->GetSelectedEntity(), e.GetPosition());
        }
      }
    }
    else if(EVENT_IS_TYPE(event, EventType::KEY_PRESS))
    {
      KeyPressEvent& e = static_cast<KeyPressEvent&>(event);
      if((e.GetButton() == GREET_KEY_LEFT_CONTROL || e.GetButton() == GREET_KEY_LEFT_SHIFT) && entityHolder.movable)
      {
        UpdateEntityPosition(entityManager->GetSelectedEntity(), Input::GetMousePos());
      }
    }
    else if(EVENT_IS_TYPE(event, EventType::KEY_RELEASE))
    {
      KeyReleaseEvent& e = static_cast<KeyReleaseEvent&>(event);
      if((e.GetButton() == GREET_KEY_LEFT_CONTROL || e.GetButton() == GREET_KEY_LEFT_SHIFT) && entityHolder.movable)
      {
        UpdateEntityPosition(entityManager->GetSelectedEntity(), Input::GetMousePos());
      }
    }
  }

  void Entity2DManager::UpdateEntityPosition(Entity entity, const Vec2f& mousePos)
  {
    Camera2DComponent& cam = scene->GetCamera2DEntity().GetComponent<Camera2DComponent>();
    Vec2f diff = cam.GetInversePVMatrix() * mousePos - cam.GetInversePVMatrix() * entityHolder.pressPos;
    if(Input::IsKeyDown(GREET_KEY_LEFT_CONTROL))
    {
      if(std::abs(diff.x) > std::abs(diff.y))
      {
        entityManager->GetSelectedEntity().GetComponent<Transform2DComponent>().position.x = entityHolder.entityPressPos.x + diff.x;
        entityManager->GetSelectedEntity().GetComponent<Transform2DComponent>().position.y = entityHolder.entityPressPos.y;
      }
      else
      {
        entityManager->GetSelectedEntity().GetComponent<Transform2DComponent>().position.x = entityHolder.entityPressPos.x;
        entityManager->GetSelectedEntity().GetComponent<Transform2DComponent>().position.y = entityHolder.entityPressPos.y + diff.y;
      }
    }
    else if(Input::IsKeyDown(GREET_KEY_LEFT_SHIFT))
    {
      Grid grid{scene->GetCamera2DEntity().GetComponent<Camera2DComponent>()};
      Vec2f pos = Vec::Round((entityHolder.entityPressPos + diff) / grid.size) * grid.size;
      entityManager->GetSelectedEntity().GetComponent<Transform2DComponent>().position = pos;
    }
    else
    {
      entityManager->GetSelectedEntity().GetComponent<Transform2DComponent>().position = entityHolder.entityPressPos + diff;
    }
    entityManager->UpdateSelectedTransform2D(NotifyOrigin::Scene);
  }

  void Entity2DManager::RenderPre() const
  {
    Grid grid{scene->GetCamera2DEntity().GetComponent<Camera2DComponent>()};

    lineBatchRenderer->SetLineWidth(1);
    lineBatchRenderer->Begin();
    lineBatchRenderer->SetPVMatrix(scene->GetCamera2DEntity().GetComponent<Camera2DComponent>().GetPVMatrix());

    for(int i = -grid.count; i < grid.count; i++)
    {
      lineBatchRenderer->DrawLine(Vec2f{grid.center.x + grid.size * i, grid.boundsMin.y},
                                  Vec2f{grid.center.x + grid.size * i, grid.boundsMax.y},
                                  Color{0xff323232});
      lineBatchRenderer->DrawLine(Vec2f{grid.boundsMin.x, grid.center.y + grid.size * i},
                                  Vec2f{grid.boundsMax.x, grid.center.y + grid.size * i},
                                  Color{0xff323232});
    }

    lineBatchRenderer->DrawLine(Vec2f{grid.boundsMin.x, 0}, Vec2f{grid.boundsMax.x, 0}, Color{0.9, 0.1, 0.1});
    lineBatchRenderer->DrawLine(Vec2f{0, grid.boundsMin.y}, Vec2f{0, grid.boundsMax.y}, Color{0.1, 0.9, 0.1});

    if(entityHolder.movable)
    {
      lineBatchRenderer->DrawLine(Vec2f{grid.boundsMin.x, entityHolder.entityPressPos.y}, Vec2f{grid.boundsMax.x, entityHolder.entityPressPos.y}, Color{0.9, 0.9, 0.9});
      lineBatchRenderer->DrawLine(Vec2f{entityHolder.entityPressPos.x, grid.boundsMin.y}, Vec2f{entityHolder.entityPressPos.x, grid.boundsMax.y}, Color{0.9, 0.9, 0.9});
    }

    lineBatchRenderer->End();
  }

  void Entity2DManager::RenderPost() const
  {
    Camera2DComponent& camera = scene->GetCamera2DEntity().GetComponent<Camera2DComponent>();
    Entity selectedEntity = entityManager->GetSelectedEntity();

    lineBatchRenderer->SetLineWidth(3);
    lineBatchRenderer->Begin();
    lineBatchRenderer->SetPVMatrix(camera.GetPVMatrix());
    if(selectedEntity.HasComponent<Transform2DComponent>())
    {
      lineBatchRenderer->DrawRectangle(entityManager->GetSelectedEntity().GetComponent<Transform2DComponent>().GetTransform(), Color{0.9, 0.5, 0.1});
    }
    lineBatchRenderer->End();
  }

  Entity Entity2DManager::GetCameraEntity()
  {
    return camera2DController.entity;
  }
}
