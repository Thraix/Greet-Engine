#include "EntityGUI.h"

#include "EntityManager.h"
#include "gui/GUITransform3D.h"

#include <graphics/gui/Button.h>
#include <graphics/gui/DropDownMenu.h>
#include <graphics/gui/TextBox.h>
#include <ecs/components/TagComponent.h>
#include <ecs/components/MeshComponent.h>
#include <utils/MetaFileLoading.h>

#include <fstream>

using namespace Greet;

EntityGUI::EntityGUI(EntityManager* entityManager, Frame* frame) :
  entityManager{entityManager}
{
  settingsContainer = frame->GetComponentByName<Container>("settings");

  treeView = frame->GetComponentByName<TreeView>("treeView");
  Button* addEntityButton = frame->GetComponentByName<Button>("addEntityButton");

  ASSERT(treeView, "Could not load Scene TreeView");
  ASSERT(addEntityButton, "Could not load Add Entity Button");

  treeNode = new TreeNode({});
  treeView->SetTreeNode(treeNode);

  treeView->SetOnNodeSelectedCallback(BIND_MEMBER_FUNC(GUITreeViewEntitySelected));
  addEntityButton->SetOnPressCallback(BIND_MEMBER_FUNC(GUIButtonCreateEntity));
  guiTransform3D = NewRef<GUITransform3D>(entityManager, settingsContainer);

  guiTransform2DComponent = ComponentFactory::GetComponent("res/guis/Transformation2DCompontent.xml", settingsContainer);
  guiMeshComponent = ComponentFactory::GetComponent("res/guis/MeshComponent.xml", settingsContainer);
  guiMeshComponent->GetComponentByName<DropDownMenu>("meshType")->SetOnSelectionChangeCallback(BIND_MEMBER_FUNC(GUIDropDownMenuMeshType));
  guiMeshComponent->GetComponentByName<TextBox>("meshPath")->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxMeshPath));
}

void EntityGUI::UpdateSelectedTransform3D()
{
  Entity entity = entityManager->GetSelectedEntity();
  if(entity.HasComponent<Transform3DComponent>())
  {
    Transform3DComponent& component = entity.GetComponent<Transform3DComponent>();
    guiTransform3D->Update(component);
  }
}

void EntityGUI::CreateEntity(const std::string& name)
{
  TreeNode* selected = treeNode;
  if(treeView->HasSelectedNode())
    selected = treeView->GetSelectedNode();
  selected->AddChildNode(TreeNode{name});
}

void EntityGUI::SelectEntity(Entity entity)
{
  settingsContainer->RemoveAllComponents();
  // TODO: select the entity in the TreeView

  if(entity.HasComponent<Transform3DComponent>())
  {
    guiTransform3D->AttachTo(settingsContainer, entity.GetComponent<Transform3DComponent>());
  }

  if(entity.HasComponent<MeshComponent>())
  {
    settingsContainer->AddComponent(guiMeshComponent);
  }
}

void EntityGUI::GUITreeViewEntitySelected(TreeView* view, TreeNode* node, bool selected)
{
  Entity entity{entityManager->GetECS().get()};

  entity = entityManager->GetECS()->Find<TagComponent>(
    [node](EntityID entity, TagComponent& tagComponent)
    {
      return tagComponent.tag == node->GetName();
    });

  ASSERT(entity, "Selected node does not exist in ECSManager");
  entityManager->SelectEntity(entity);
}

void EntityGUI::GUIButtonCreateEntity(Component* component)
{
  entityManager->CreateEntity();
}

void EntityGUI::GUIDropDownMenuMeshType(Greet::Component* component, const std::string& oldLabel, const std::string& newLabel)
{
  meshType = newLabel;
  if(meshType != "model" || FileUtils::FileExist(newLabel))
  {
    if(entityManager->GetSelectedEntity())
    {
      MetaFileClass meta{
        {{"mesh", meshType},
         {"mesh-path", meshPath}}
      };
      Ref<Mesh> mesh = MetaFileLoading::LoadMesh(meta, "mesh");
      entityManager->UpdateSelectedMesh(mesh);
    }
  }
}

void EntityGUI::GUITextBoxMeshPath(Greet::Component* component, const std::string& oldLabel, const std::string& newLabel)
{
  meshPath = newLabel;
  if(meshType != "model" || FileUtils::FileExist(newLabel))
  {
    if(entityManager->GetSelectedEntity())
    {
      MetaFileClass meta{
        {{"mesh", meshType},
         {"mesh-path", meshPath}}
      };
      Ref<Mesh> mesh = MetaFileLoading::LoadMesh(meta, "mesh");
      entityManager->UpdateSelectedMesh(mesh);
    }
  }
}
