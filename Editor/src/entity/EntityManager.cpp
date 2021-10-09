#include "EntityManager.h"

#include <ecs/Entity.h>
#include <graphics/gui/Container.h>
#include <graphics/gui/Frame.h>
#include <graphics/gui/TreeNode.h>
#include <graphics/gui/TreeView.h>

using namespace Greet;

EntityManager::EntityManager(Frame* frame) :
  ecs{NewRef<ECSManager>()}, scene{NewRef<EditorScene>(this)}
{
  // TODO: Move all gui handling to its own class
  settingsContainer = frame->GetComponentByName<Container>("settings");
  settingsContainer->AddComponent(ComponentFactory::GetComponent("res/guis/Transformation3DCompontent.xml", settingsContainer));
  settingsContainer->AddComponent(ComponentFactory::GetComponent("res/guis/Transformation2DCompontent.xml", settingsContainer));
  settingsContainer->LoadFrameStyle(frame->GetStylingFile());

  sceneView = frame->GetComponentByName<SceneView>("sceneView");
  sceneTreeView = frame->GetComponentByName<TreeView>("treeView");
  Button* addEntityButton = frame->GetComponentByName<Button>("addEntityButton");

  ASSERT(sceneTreeView, "Could not load Scene TreeView");
  ASSERT(addEntityButton, "Could not load Add Entity Button");
  ASSERT(sceneView, "Could not load Scene View");

  sceneView->GetSceneManager().Add3DScene(scene, "scene");

  sceneTree = new TreeNode({});
  sceneTreeView->SetTreeNode(sceneTree);

  addEntityButton->SetOnPressCallback([this](Component* component) { CreateEntity(); } );

  // TODO: Remove debug stuff
  CreateEntity();
  CreateEntity();
  CreateEntity();
}

EntityManager::~EntityManager()
{
  sceneView->GetSceneManager().Remove3DScene("scene");
}

void EntityManager::SelectEntity(Entity entity)
{
  selectedEntity = entity;
  scene->SelectEntity(entity);
}

void EntityManager::CreateEntity()
{
  static int entityId = 0;
  std::string name = "Entity#" + std::to_string(++entityId);

  TreeNode* selected = sceneTreeView->GetRootTreeNode();
  if(sceneTreeView->HasSelectedNode())
    selected = sceneTreeView->GetSelectedNode();
  selected->AddChildNode(TreeNode{name});

  Entity entity = Entity::Create(ecs.get());
  entity.AddComponent<TagComponent>(name);

  entity.AddComponent<Transform3DComponent>(Vec3f{entityId * 1.5f, 0, 0}, Vec3f{1,1,1}, Vec3f{0,0,0});
  MeshData data = MeshFactory::Cube({0,0,0}, {1,2,1.5});
  entity.AddComponent<MeshComponent>(NewRef<Mesh>(data));
  entity.AddComponent<MaterialComponent>(Material{ShaderFactory::Shader3D()});
}

Greet::Entity EntityManager::GetSelectedEntity() const
{
  return selectedEntity;
}

const Ref<ECSManager>& EntityManager::GetECS() const
{
  return ecs;
}
