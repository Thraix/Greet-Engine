#include "EntityScene.h"

#include "EntityManager.h"
#include "Entity2DManager.h"
#include "EntityGUI.h"
#include "EntityRendering.h"

#include <ecs/components/ColorComponent.h>
#include <ecs/components/MaterialComponent.h>
#include <ecs/components/MeshComponent.h>
#include <ecs/components/SpriteComponent.h>
#include <ecs/components/TagComponent.h>
#include <ecs/components/Transform2DComponent.h>
#include <ecs/components/Transform3DComponent.h>
#include <ecs/components/UUIDComponent.h>
#include <event/KeyEvent.h>
#include <event/MouseEvent.h>
#include <input/InputDefines.h>

namespace Greet
{

  EntityScene::EntityScene(EntityManager* entityManager)
    : ECSScene{entityManager->GetECS(), "res/scenes/game.meta"},
      entityManager{entityManager},
      editorEcs{NewRef<ECSManager>()},
      entity2DManager{NewRef<Entity2DManager>(entityManager, this)},
      camera3DController{entityManager->GetECS().get()}
  {
    ASSERT(manager.get() != nullptr, "ECSManager was not initialized");
    entityRendering = NewRef<EntityRendering>(manager.get());
  }

  void EntityScene::Update(float timeElapsed)
  {
    switch(activeScene)
    {
      case ActiveScene::_2D: break;
      case ActiveScene::_3D: camera3DController.OnUpdate(timeElapsed); break;
      case ActiveScene::Runtime: ECSScene::Update(timeElapsed); break;
    }
  }

  void EntityScene::Render() const
  {
    switch(activeScene)
    {
      case ActiveScene::_2D: Render2D(); break;
      case ActiveScene::_3D: ECSScene::Render3DScene(); break;
      case ActiveScene::Runtime: ECSScene::Render(); break;
    }
  }

  void EntityScene::Render2D() const
  {
    entity2DManager->RenderPre();
    ECSScene::Render2D();
    entity2DManager->RenderPost();
  }

  void EntityScene::Render3D(const Camera3DComponent& cam, const Environment3DComponent& env) const
  {
    ECSScene::Render3D(cam, env);

    entityRendering->RenderOutlines(cam, entityManager->GetSelectedEntity());
    entityRendering->RenderGizmo(cam, entityManager->GetSelectedEntity());
  }

  void EntityScene::OnEvent(Event& event)
  {
    if(activeScene == ActiveScene::_2D || activeScene == ActiveScene::_3D)
    {
      if(EVENT_IS_TYPE(event, EventType::KEY_PRESS))
      {
        KeyPressEvent& e = static_cast<KeyPressEvent&>(event);
        if(e.GetButton() == GREET_KEY_DELETE && entityManager->GetSelectedEntity())
        {
          entityManager->DeleteEntity(entityManager->GetSelectedEntity());
        }
      }
      else if(EVENT_IS_TYPE(event, EventType::VIEWPORT_RESIZE))
      {
        ECSScene::OnEvent(event);
      }
    }
    switch(activeScene)
    {
      case ActiveScene::_2D: entity2DManager->OnEvent(event); break;
      case ActiveScene::_3D: OnEvent3D(event); break;
      case ActiveScene::Runtime: ECSScene::OnEvent(event); break;
    }
  }

  void EntityScene::OnEvent3D(Event& event)
  {
    camera3DController.OnEvent(event);
    Entity camera = GetCamera3DEntity();

    if(!camera)
      return;

    Camera3DComponent& cameraComponent = camera.GetComponent<Camera3DComponent>();
    Entity selectedEntity = entityManager->GetSelectedEntity();

    if(selectedEntity && selectedEntity.HasComponent<Transform3DComponent>())
    {
      TransformGizmo& gizmo = entityRendering->GetTransformGizmo();

      bool handled = gizmo.OnEvent(event, cameraComponent);
      if(handled)
      {
        Transform3DComponent& transform = selectedEntity.GetComponent<Transform3DComponent>();
        transform.SetPosition(gizmo.position);
        transform.SetScale(gizmo.scale);
        transform.SetRotation(gizmo.rotation);
        entityManager->UpdateSelectedTransform3D(NotifyOrigin::Scene);
        event.AddFlag(EVENT_HANDLED);
        return;
      }
    }

    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      if(e.GetButton() != GREET_MOUSE_1)
        return;

      Entity collisionEntity = GetNearestRaycastedEntity(cameraComponent, e.GetPosition());
      if(collisionEntity)
        event.AddFlag(EVENT_HANDLED);
      if(selectedEntity != collisionEntity)
      {
        entityManager->SelectEntity(collisionEntity);
      }
    }
  }

  void EntityScene::SelectEntity(Entity entity)
  {
    if(entity.HasComponent<Transform3DComponent>())
    {
      Transform3DComponent& transform = entity.GetComponent<Transform3DComponent>();
      TransformGizmo& gizmo = entityRendering->GetTransformGizmo();
      gizmo.position = transform.GetPosition();
      gizmo.scale = transform.GetScale();
      gizmo.rotation = transform.GetRotation();
    }
  }

  void EntityScene::UpdateSelectedTransform3D()
  {
    if(entityManager->GetSelectedEntity().HasComponent<Transform3DComponent>())
    {
      // TODO: Can't we render this where the entity is every time instead?
      //       Seems unnecessary to update when changed
      Transform3DComponent& transform = entityManager->GetSelectedEntity().GetComponent<Transform3DComponent>();
      TransformGizmo& gizmo = entityRendering->GetTransformGizmo();
      gizmo.position = transform.GetPosition();
      gizmo.scale = transform.GetScale();
      gizmo.rotation = transform.GetRotation();
    }
  }

  Entity EntityScene::GetCamera2DEntity() const
  {
    if(activeScene == ActiveScene::Runtime)
    {
      return ECSScene::GetCamera2DEntity();
    }
    else
    {
      return entity2DManager->GetCameraEntity();
    }
  }

  Entity EntityScene::GetCamera3DEntity() const
  {
    if(activeScene == ActiveScene::Runtime)
    {
      return ECSScene::GetCamera3DEntity();
    }
    else
    {
      return camera3DController.entity;
    }
  }

  Entity EntityScene::GetEnvironment2DEntity() const
  {
    if(activeScene == ActiveScene::Runtime)
    {
      return ECSScene::GetEnvironment2DEntity();
    }
    else
    {
      return entity2DManager->GetCameraEntity();
    }
  }

  Entity EntityScene::GetEnvironment3DEntity() const
  {
    if(activeScene == ActiveScene::Runtime)
    {
      return ECSScene::GetEnvironment3DEntity();
    }
    else
    {
      return camera3DController.entity;
    }
  }

  Entity EntityScene::GetNearestRaycastedEntity(Camera3DComponent& cameraComponent, const Vec2f& pos, float farDistance)
  {
    Entity collisionEntity{manager.get()};
    manager->Each<Transform3DComponent, MeshComponent, MaterialComponent>(
      [&](EntityID entity, Transform3DComponent& transform, MeshComponent& mesh, MaterialComponent& material)
      {
        Line line = cameraComponent.GetScreenToWorldCoordinate(pos);
        std::pair<bool, float> collision = mesh.mesh->GetBoundingBox().LineIntersects(transform.transform, line);
        if(collision.first && collision.second < farDistance)
        {
          collisionEntity = entity;
          farDistance = collision.second;
        }
      });
    return collisionEntity;
  }

  Ref<ECSManager> EntityScene::GetEditorECS()
  {
    return editorEcs;
  }
}
