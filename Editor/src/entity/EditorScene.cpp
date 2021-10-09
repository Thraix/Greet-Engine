#include "EditorScene.h"

#include "EntityManager.h"

#include "../scripts/CameraController.h"

using namespace Greet;

EditorScene::EditorScene(EntityManager* entityManager)
  : ECSScene{entityManager->GetECS()}, entityManager{entityManager}
{
  ASSERT(manager.get() != nullptr, "ECSManager was not initialized");
  entityRendering = Greet::NewRef<EntityRendering>(manager.get());

  Entity camera = AddEntity("Camera3D");
  camera.AddComponent<Camera3DComponent>(Mat4::Identity(), 90, 0.01, 1000, true);
  camera.AddComponent<Environment3DComponent>(ShaderFactory::ShaderSkybox(), TextureManager::LoadCubeMap("res/textures/skybox.meta"));
  camera.AddComponent<NativeScriptComponent>(NewRef<NativeScriptHandler>(new CameraControllerScript({0,0,0}, {M_PI / 4,M_PI / 4,0})));

}

void EditorScene::Render3D(const Camera3DComponent& cam, const Environment3DComponent& env) const
{
  ECSScene::Render3D(cam, env);

  entityRendering->RenderOutlines(cam, entityManager->GetSelectedEntity());
  entityRendering->RenderGizmo(cam, entityManager->GetSelectedEntity());
}

void EditorScene::OnEvent(Event& event)
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

void EditorScene::SelectEntity(Greet::Entity entity)
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

Entity EditorScene::GetCameraEntity() const
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

Entity EditorScene::GetNearestRaycastedEntity(Camera3DComponent& cameraComponent, const Vec2f& pos, float farDistance)
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
