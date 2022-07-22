#include "Entity2DManager.h"

using namespace Greet;

#include "EntityManager.h"

#include <input/Input.h>
#include <event/MouseEvent.h>

#include <ecs/components/Camera2DComponent.h>
#include <ecs/components/Environment2DComponent.h>
#include <ecs/components/Transform2DComponent.h>
#include <ecs/components/ColorComponent.h>
#include <ecs/components/SpriteComponent.h>


Entity2DManager::Entity2DManager(EntityManager* entityManager, ECSScene* scene)
  : entityManager{entityManager}, scene{scene}, lineBatchRenderer{NewRef<LineBatchRenderer>()}
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
      if(pressedEntity != entityManager->GetSelectedEntity())
      {
        entityManager->SelectEntity(pressedEntity);
      }
      if(pressedEntity)
      {
        entityHolder.holding = true;
        entityHolder.threshold = false;
        entityHolder.entityPressPos = pressedEntity.GetComponent<Transform2DComponent>().position;
        entityHolder.pressPos = e.GetPosition();
      }
    }
  }
  else if(EVENT_IS_TYPE(event, EventType::MOUSE_RELEASE))
  {
    entityHolder.holding = false;
  }
  else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
  {
    if(entityHolder.holding)
    {
      MouseMoveEvent& e = static_cast<MouseMoveEvent&>(event);
      Camera2DComponent& cam = scene->GetCamera2DEntity().GetComponent<Camera2DComponent>();
      if(!entityHolder.threshold)
      {
        Mat3 invViewportMatrix = ~Mat3::OrthographicViewport();
        Vec2f oldMousePos = invViewportMatrix * entityHolder.pressPos;
        Vec2f newMousePos = invViewportMatrix * e.GetPosition();
        if((newMousePos - oldMousePos).LengthSQ() > 10 * 10)
        {
          entityHolder.threshold = true;
        }
      }
      else
      {
        Vec2f oldMousePos = cam.GetInversePVMatrix() * entityHolder.pressPos;
        Vec2f newMousePos = cam.GetInversePVMatrix() * e.GetPosition();
        Vec2f entityPos = entityHolder.entityPressPos + (newMousePos - oldMousePos);
        entityManager->GetSelectedEntity().GetComponent<Transform2DComponent>().position = entityPos;
        entityManager->UpdateSelectedTransform2D(NotifyOrigin::Scene);
      }
    }
  }
}

void Entity2DManager::Render() const
{
  lineBatchRenderer->Begin();
  lineBatchRenderer->SetPVMatrix(scene->GetCamera2DEntity().GetComponent<Camera2DComponent>().GetPVMatrix());
  if(entityManager->GetSelectedEntity().HasComponent<Transform2DComponent>())
  {
    lineBatchRenderer->DrawRectangle(entityManager->GetSelectedEntity().GetComponent<Transform2DComponent>().GetTransform(), Color{0.8, 0.2, 0.8});
  }
  lineBatchRenderer->End();
}
