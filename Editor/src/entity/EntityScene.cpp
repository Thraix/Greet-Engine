#include "EntityScene.h"

#include "EntityManager.h"
#include "EntityRendering.h"
#include "gui/GUITransform3D.h"

#include <ecs/components/Camera2DComponent.h>
#include <ecs/components/Camera3DComponent.h>
#include <ecs/components/MaterialComponent.h>
#include <ecs/components/MeshComponent.h>
#include <ecs/components/NativeScriptComponent.h>
#include <ecs/components/Transform3DComponent.h>
#include <ecs/components/UUIDComponent.h>
#include <graphics/shaders/ShaderFactory.h>
#include <graphics/textures/TextureManager.h>
#include <input/InputDefines.h>
#include <scripting/NativeScriptHandler.h>
#include <utils/UUID.h>

using namespace Greet;

const UUID CAMERA_2D_UUID{"a4396de6-dbd9-6dc4-1e1d-3e662b62ddab"};
const UUID CAMERA_3D_UUID{"862f1b54-649c-3775-a53d-5834a7e1135b"};

EntityScene::EntityScene(EntityManager* entityManager)
  : ECSScene{entityManager->GetECS(), "res/scenes/editor_scene.meta"}, entityManager{entityManager}
{
  ASSERT(manager.get() != nullptr, "ECSManager was not initialized");
  entityRendering = Greet::NewRef<EntityRendering>(manager.get());
}

void EntityScene::Render() const
{
  switch(activeScene)
  {
    case ActiveScene::_2D: ECSScene::Render2D(); break;
    case ActiveScene::_3D: ECSScene::Render3DScene(); break;
  }
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
    case ActiveScene::_2D: OnEvent2D(event); break;
    case ActiveScene::_3D: OnEvent3D(event); break;
  }
}

void EntityScene::OnEvent2D(Greet::Event& event)
{
}

void EntityScene::OnEvent3D(Greet::Event& event)
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
    MousePressEvent e = static_cast<MousePressEvent&>(event);
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

void EntityScene::SelectEntity(Greet::Entity entity)
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

void EntityScene::UpdateSelectedTransform2D()
{
  if(entityManager->GetSelectedEntity().HasComponent<Transform3DComponent>())
  {
    // TODO: Update gizmos when they are implemented
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

  if(!camera)
    Log::Warning("Editor Camera2D not found in scene");
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

  if(!camera)
    Log::Warning("Editor Camera3D not found in scene");
  return camera;
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
