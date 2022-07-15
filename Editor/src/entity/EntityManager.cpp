#include "EntityManager.h"

#include "EntityScene.h"
#include "EntityGUI.h"

#include <ecs/Entity.h>
#include <ecs/components/MaterialComponent.h>
#include <ecs/components/MeshComponent.h>
#include <ecs/components/TagComponent.h>
#include <ecs/components/Transform2DComponent.h>
#include <ecs/components/Transform3DComponent.h>
#include <ecs/components/ColorComponent.h>
#include <ecs/components/SpriteComponent.h>
#include <ecs/components/UUIDComponent.h>
#include <graphics/gui/SceneView.h>
#include <graphics/models/MeshFactory.h>
#include <graphics/shaders/ShaderFactory.h>

using namespace Greet;

EntityManager::EntityManager(Frame* frame) :
  ecs{NewRef<ECSManager>()},
  selectedEntity{ecs.get()},
  scene{NewRef<EntityScene>(this)},
  gui{NewRef<EntityGUI>(this, frame)}
{
  sceneView = frame->GetComponentByName<SceneView>("sceneView");
  ASSERT(sceneView, "Could not load Scene View");
  sceneView->GetSceneManager().Add3DScene(scene, "scene");

  // TODO: Remove debug stuff
  CreateEntity2D();
  CreateEntity2D();
  CreateEntity2D();
}

EntityManager::~EntityManager()
{
  sceneView->GetSceneManager().Remove3DScene("scene");
}

void EntityManager::SelectEntity(Entity entity)
{
  selectedEntity = entity.GetID();
  scene->SelectEntity(entity);
  gui->SelectEntity(entity);
}

void EntityManager::CreateEntity2D()
{
  Entity entity = Entity::Create(ecs.get());
  std::string name = "Entity#" + std::to_string(++entityId);

  gui->CreateEntity(name);

  entity.AddComponent<UUIDComponent>(UUID{});
  entity.AddComponent<TagComponent>(name);

  entity.AddComponent<Transform2DComponent>(Vec2f{entityId * 150.0f, 80}, Vec2f{100,100}, 0);
  entity.AddComponent<ColorComponent>(0xffffffff);
}

void EntityManager::CreateEntity3D()
{
  std::string name = "Entity#" + std::to_string(++entityId);

  gui->CreateEntity(name);

  Entity entity = Entity::Create(ecs.get());
  entity.AddComponent<UUIDComponent>(UUID{});
  entity.AddComponent<TagComponent>(name);

  entity.AddComponent<Transform3DComponent>(Vec3f{entityId * 1.5f, 0, 0}, Vec3f{1,1,1}, Vec3f{0,0,0});
  MeshData data = MeshFactory::Cube({0,0,0}, {1,2,1.5});
  entity.AddComponent<MeshComponent>(NewRef<Mesh>(data));
  entity.AddComponent<MaterialComponent>(Material{ShaderFactory::Shader3D()});
}

void EntityManager::UpdateSelectedTransform3D(NotifyOrigin notifyOrigin)
{
  if(notifyOrigin != NotifyOrigin::GUI)
    gui->UpdateSelectedTransform3D();
  if(notifyOrigin != NotifyOrigin::Scene)
    scene->UpdateSelectedTransform3D();
}

void EntityManager::UpdateSelectedTransform2D(NotifyOrigin notifyOrigin)
{
  if(notifyOrigin != NotifyOrigin::GUI)
    gui->UpdateSelectedTransform2D();
  if(notifyOrigin != NotifyOrigin::Scene)
    scene->UpdateSelectedTransform2D();
}

void EntityManager::UpdateSelectedMesh(const Ref<Mesh>& mesh)
{
  selectedEntity.GetComponent<MeshComponent>().mesh = mesh;
}

Greet::Entity EntityManager::GetSelectedEntity() const
{
  return selectedEntity;
}

const Ref<ECSManager>& EntityManager::GetECS() const
{
  return ecs;
}
