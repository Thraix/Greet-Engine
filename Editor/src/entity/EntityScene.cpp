#include "EntityScene.h"

#include "EntityManager.h"
#include "Entity2DManager.h"
#include "EntityGUI.h"
#include "EntityRendering.h"
#include "gui/GUITransform3D.h"
#include "../scripts/Camera2DControllerScript.cpp"
#include "../scripts/Camera3DControllerScript.cpp"

#include <ecs/components/Camera2DComponent.h>
#include <ecs/components/Camera3DComponent.h>
#include <ecs/components/ColorComponent.h>
#include <ecs/components/Environment2DComponent.h>
#include <ecs/components/Environment3DComponent.h>
#include <ecs/components/MaterialComponent.h>
#include <ecs/components/MeshComponent.h>
#include <ecs/components/NativeScriptComponent.h>
#include <ecs/components/SpriteComponent.h>
#include <ecs/components/TagComponent.h>
#include <ecs/components/Transform2DComponent.h>
#include <ecs/components/Transform3DComponent.h>
#include <ecs/components/UUIDComponent.h>
#include <graphics/shaders/ShaderFactory.h>
#include <graphics/textures/TextureManager.h>
#include <input/InputDefines.h>
#include <scripting/NativeScriptHandler.h>
#include <utils/UUID.h>

namespace Greet
{
  const UUID CAMERA_2D_UUID{"a4396de6-dbd9-6dc4-1e1d-3e662b62ddab"};
  const UUID CAMERA_3D_UUID{"862f1b54-649c-3775-a53d-5834a7e1135b"};

  EntityScene::EntityScene(EntityManager* entityManager)
    : ECSScene{entityManager->GetECS(), "res/scenes/game.meta"},
      entityManager{entityManager},
      entity2DManager{NewRef<Entity2DManager>(entityManager, this)}
  {
    ASSERT(manager.get() != nullptr, "ECSManager was not initialized");
    entityRendering = NewRef<EntityRendering>(manager.get());

    manager->Each<TagComponent, UUIDComponent>([&](EntityID entity, TagComponent& tag, UUIDComponent& uuid)
    {
      entityManager->GetEntityGUI()->CreateEntity(Entity{manager.get(), entity});
    });

    Entity camera2D = Entity{manager.get(), manager->CreateEntity()};
    camera2D.AddComponent<UUIDComponent>(CAMERA_2D_UUID);
    camera2D.AddComponent<TagComponent>("EditorCamera2D");
    camera2D.AddComponent<Camera2DComponent>(Vec2f{0, 0}, Vec2f{1, 1}, 0, false);
    camera2D.AddComponent<Environment2DComponent>(ShaderFactory::Shader2D());
    camera2D.AddComponent<NativeScriptComponent>(NewRef<NativeScriptHandler>(new Camera2DControllerScript{}));

    Entity camera3D = Entity{manager.get(), manager->CreateEntity()};
    camera3D.AddComponent<UUIDComponent>(CAMERA_3D_UUID);
    camera3D.AddComponent<TagComponent>("EditorCamera3D");
    camera3D.AddComponent<Camera3DComponent>(Mat4::Identity(), 90, 0.001, 1000.0f, false);
    camera3D.AddComponent<Environment3DComponent>(ShaderFactory::ShaderSkybox(), TextureManager::LoadCubeMap("res/textures/skybox.meta"));
    camera3D.AddComponent<NativeScriptComponent>(NewRef<NativeScriptHandler>(new Camera3DControllerScript{}));
  }

  void EntityScene::Render() const
  {
    switch(activeScene)
    {
      case ActiveScene::_2D: Render2D(); break;
      case ActiveScene::_3D: ECSScene::Render3DScene(); break;
      case ActiveScene::Runtime: break;
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
    ECSScene::OnEvent(event);
    switch(activeScene)
    {
      case ActiveScene::_2D: entity2DManager->OnEvent(event); break;
      case ActiveScene::_3D: OnEvent3D(event); break;
      case ActiveScene::Runtime: break;
    }
  }

  void EntityScene::OnEvent3D(Event& event)
  {
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
    Entity camera{manager.get()};
    camera = manager->Find<Camera2DComponent, UUIDComponent>(
      [&](EntityID id, Camera2DComponent& cam, UUIDComponent& uuid)
      {
        return uuid.uuid == CAMERA_2D_UUID;
      });

    ASSERT(camera, "Editor Camera2D not found in scene");
    return camera;
  }

  Entity EntityScene::GetCamera3DEntity() const
  {
    Entity camera{manager.get()};
    camera = manager->Find<Camera3DComponent, UUIDComponent>(
      [&](EntityID id, Camera3DComponent& cam, UUIDComponent& uuid)
      {
        return uuid.uuid == CAMERA_3D_UUID;
      });

    ASSERT(camera, "Editor Camera3D not found in scene");
    return camera;
  }

  Entity EntityScene::GetEnvironment2DEntity() const
  {
    Entity environment{manager.get()};
    environment = manager->Find<Environment2DComponent, UUIDComponent>(
      [&](EntityID id, Environment2DComponent& cam, UUIDComponent& uuid)
      {
        return uuid.uuid == CAMERA_2D_UUID;
      });

    ASSERT(environment, "Editor Environment2D not found in scene");
    return environment;
  }

  Entity EntityScene::GetEnvironment3DEntity() const
  {
    Entity environment{manager.get()};
    environment = manager->Find<Environment3DComponent, UUIDComponent>(
      [&](EntityID id, Environment3DComponent& cam, UUIDComponent& uuid)
      {
        return uuid.uuid == CAMERA_3D_UUID;
      });

    ASSERT(environment, "Editor Environment3D not found in scene");
    return environment;
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
}
