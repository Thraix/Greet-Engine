#include "EntityScene.h"

#include "../scripts/CameraController.h"
#include "EntityManager.h"
#include "EntityRendering.h"
#include "gui/GUITransform3D.h"

#include <ecs/components/MaterialComponent.h>
#include <ecs/components/MeshComponent.h>
#include <ecs/components/NativeScriptComponent.h>
#include <ecs/components/Transform3DComponent.h>
#include <graphics/shaders/ShaderFactory.h>
#include <graphics/textures/TextureManager.h>
#include <scripting/NativeScriptHandler.h>

using namespace Greet;

EntityScene::EntityScene(EntityManager* entityManager)
  : ECSScene{entityManager->GetECS()}, entityManager{entityManager}
{
  ASSERT(manager.get() != nullptr, "ECSManager was not initialized");
  entityRendering = Greet::NewRef<EntityRendering>(manager.get());

  Entity camera = AddEntity("Camera3D");
  camera.AddComponent<Camera3DComponent>(Mat4::Identity(), 90, 0.01, 1000, true);
  camera.AddComponent<Environment3DComponent>(ShaderFactory::ShaderSkybox(), TextureManager::LoadCubeMap("res/textures/skybox.meta"));
  camera.AddComponent<NativeScriptComponent>(NewRef<NativeScriptHandler>(new CameraControllerScript({0,0,0}, {M_PI / 4,M_PI / 4,0})));

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
  Entity camera = GetCameraEntity();

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

void EntityScene::UpdateSelectedTransform3D()
{
  if(entityManager->GetSelectedEntity().HasComponent<Transform3DComponent>())
  {
    Transform3DComponent& transform = entityManager->GetSelectedEntity().GetComponent<Transform3DComponent>();
    TransformGizmo& gizmo = entityRendering->GetTransformGizmo();
    gizmo.position = transform.GetPosition();
    gizmo.scale = transform.GetScale();
    gizmo.rotation = transform.GetRotation();
  }
}

Entity EntityScene::GetCameraEntity() const
{
  Entity camera{manager.get()};
  camera = manager->Find<Camera3DComponent>(
    [&](EntityID id, Camera3DComponent& cam)
    {
      return cam.active;
    });

  if(!camera)
    Log::Warning("No camera in scene");
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
