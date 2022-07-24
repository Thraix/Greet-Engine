#include "Entity2DManager.h"

using namespace Greet;

#include "EntityManager.h"

#include <input/Input.h>
#include <event/MouseEvent.h>
#include <event/KeyEvent.h>
#include <graphics/RenderCommand.h>
#include <graphics/textures/TextureManager.h>
#include <graphics/shaders/ShaderFactory.h>

#include <ecs/components/Camera2DComponent.h>
#include <ecs/components/Environment2DComponent.h>
#include <ecs/components/Transform2DComponent.h>
#include <ecs/components/ColorComponent.h>
#include <ecs/components/SpriteComponent.h>


Entity2DManager::Entity2DManager(EntityManager* entityManager, ECSScene* scene)
  : entityManager{entityManager}, scene{scene},
    lineBatchRenderer{NewRef<LineBatchRenderer>()}
{}

void Entity2DManager::OnEvent(Greet::Event& event)
{
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
    if(e.GetButton() == GREET_KEY_LEFT_CONTROL && entityHolder.movable)
    {
      UpdateEntityPosition(entityManager->GetSelectedEntity(), Input::GetMousePos());
    }
  }
  else if(EVENT_IS_TYPE(event, EventType::KEY_RELEASE))
  {
    KeyReleaseEvent& e = static_cast<KeyReleaseEvent&>(event);
    if(e.GetButton() == GREET_KEY_LEFT_CONTROL && entityHolder.movable)
    {
      UpdateEntityPosition(entityManager->GetSelectedEntity(), Input::GetMousePos());
    }
  }
}
void Entity2DManager::UpdateEntityPosition(Greet::Entity entity, const Greet::Vec2f& mousePos)
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
  else
  {
    entityManager->GetSelectedEntity().GetComponent<Transform2DComponent>().position = entityHolder.entityPressPos + diff;
  }
  entityManager->UpdateSelectedTransform2D(NotifyOrigin::Scene);
}

void Entity2DManager::RenderPre() const
{
  const float MIN_GRID_SIZE = 20;
  Mat3 invPVMatrix = scene->GetCamera2DEntity().GetComponent<Camera2DComponent>().GetInversePVMatrix();
  Vec2f worldCoordMin = invPVMatrix * Vec2f{-1, -1};
  Vec2f worldCoordMax = invPVMatrix * Vec2f{1, 1};
  Vec2f worldCoordSize = (worldCoordMax - worldCoordMin).Abs();

  Vec2f viewportSize = Vec2f{RenderCommand::GetViewportWidth(), RenderCommand::GetViewportHeight()};
  float gridCount = viewportSize.x / MIN_GRID_SIZE;
  float distanceWorldCoord = pow(10, ceil(log10(worldCoordSize.x / gridCount)));
  Vec2f center = invPVMatrix * Vec2f{0, 0};
  Vec2f gridCenter = Vec2f{ceil(center.x / distanceWorldCoord) * distanceWorldCoord, ceil(center.y / distanceWorldCoord) * distanceWorldCoord};

  lineBatchRenderer->SetLineWidth(1);
  lineBatchRenderer->Begin();
  lineBatchRenderer->SetPVMatrix(scene->GetCamera2DEntity().GetComponent<Camera2DComponent>().GetPVMatrix());

  for(int i = -floor(gridCount); i < ceil(gridCount); i++)
  {
    lineBatchRenderer->DrawLine(Vec2f{gridCenter.x + distanceWorldCoord * i, worldCoordMin.y},
                                Vec2f{gridCenter.x + distanceWorldCoord * i, worldCoordMax.y},
                                Color{0xff323232});
    lineBatchRenderer->DrawLine(Vec2f{worldCoordMin.x, gridCenter.y + distanceWorldCoord * i},
                                Vec2f{worldCoordMax.x, gridCenter.y + distanceWorldCoord * i},
                                Color{0xff323232});
  }

  lineBatchRenderer->DrawLine(Vec2f{worldCoordMin.x, 0}, Vec2f{worldCoordMax.x, 0}, Color{0.9, 0.1, 0.1});
  lineBatchRenderer->DrawLine(Vec2f{0, worldCoordMin.y}, Vec2f{0, worldCoordMax.y}, Color{0.1, 0.9, 0.1});

  if(entityHolder.movable && Input::IsKeyDown(GREET_KEY_LEFT_CONTROL))
  {
    lineBatchRenderer->DrawLine(Vec2f{worldCoordMin.x, entityHolder.entityPressPos.y}, Vec2f{worldCoordMax.x, entityHolder.entityPressPos.y}, Color{0.9, 0.9, 0.9});
    lineBatchRenderer->DrawLine(Vec2f{entityHolder.entityPressPos.x, worldCoordMin.y}, Vec2f{entityHolder.entityPressPos.x, worldCoordMax.y}, Color{0.9, 0.9, 0.9});
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
