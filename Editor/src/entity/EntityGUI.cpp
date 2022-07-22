#include "EntityGUI.h"

#include "EntityManager.h"
#include "gui/GUITransform3D.h"
#include "gui/GUITransform2D.h"
#include "gui/GUISprite.h"

#include <graphics/gui/Button.h>
#include <graphics/gui/DropDownMenu.h>
#include <graphics/gui/TextBox.h>
#include <graphics/gui/ColorPicker.h>
#include <ecs/components/TagComponent.h>
#include <ecs/components/SpriteComponent.h>
#include <ecs/components/UUIDComponent.h>
#include <ecs/components/MeshComponent.h>
#include <ecs/components/ColorComponent.h>
#include <utils/MetaFileLoading.h>

#include <fstream>

using namespace Greet;

EntityGUI::EntityGUI(EntityManager* entityManager, Frame* frame) :
  entityManager{entityManager}
{
  settingsContainer = frame->GetComponentByName<Container>("settings");

  treeView = frame->GetComponentByName<TreeView>("treeView");
  Button* addEntity2DButton = frame->GetComponentByName<Button>("addEntity2DButton");
  Button* addEntity3DButton = frame->GetComponentByName<Button>("addEntity3DButton");

  ASSERT(treeView, "Could not load Scene TreeView");
  ASSERT(addEntity2DButton, "Could not load Add Entity Button");
  ASSERT(addEntity3DButton, "Could not load Add Entity Button");

  treeNode = new TreeNode({});
  treeView->SetTreeNode(treeNode);

  treeView->SetOnNodeSelectedCallback(BIND_MEMBER_FUNC(GUITreeViewEntitySelected));
  addEntity2DButton->SetOnPressCallback(BIND_MEMBER_FUNC(GUIButtonCreateEntity2D));
  addEntity3DButton->SetOnPressCallback(BIND_MEMBER_FUNC(GUIButtonCreateEntity3D));
  guiTransform2D = NewRef<GUITransform2D>(entityManager, settingsContainer);
  guiTransform3D = NewRef<GUITransform3D>(entityManager, settingsContainer);
  guiSprite = NewRef<GUISprite>(entityManager, settingsContainer);

  guiTagComponent = ComponentFactory::GetComponent("res/guis/TagComponent.xml", settingsContainer);
  guiTagComponent->GetComponentByName<TextBox>("tag")->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxTagName));

  guiColorComponent = ComponentFactory::GetComponent("res/guis/ColorComponent.xml", settingsContainer);
  guiColorComponent->GetComponentByName<ColorPicker>("color")->SetOnColorChangeCallback(BIND_MEMBER_FUNC(GUIColorPickerColor));

  guiAddComponent = static_cast<DropDownMenu*>(ComponentFactory::GetComponent("res/guis/AddComponent.xml", settingsContainer));
  guiAddComponent->SetOnSelectionChangeCallback(BIND_MEMBER_FUNC(GUIDropDownMenuAddComponent));

  guiMeshComponent = ComponentFactory::GetComponent("res/guis/MeshComponent.xml", settingsContainer);
  guiMeshComponent->GetComponentByName<DropDownMenu>("meshType")->SetOnSelectionChangeCallback(BIND_MEMBER_FUNC(GUIDropDownMenuMeshType));
  guiMeshComponent->GetComponentByName<TextBox>("meshPath")->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxMeshPath));
}

EntityGUI::~EntityGUI()
{
  settingsContainer->RemoveAllComponents();
  delete guiTagComponent;
  delete guiColorComponent;
  delete guiMeshComponent;
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

void EntityGUI::UpdateSelectedTransform2D()
{
  Entity entity = entityManager->GetSelectedEntity();
  if(entity.HasComponent<Transform2DComponent>())
  {
    Transform2DComponent& component = entity.GetComponent<Transform2DComponent>();
    guiTransform2D->Update(component);
  }
}

void EntityGUI::CreateEntity(Entity entity)
{
  TreeNode* selected = treeNode;
  if(treeView->HasSelectedNode())
    selected = treeView->GetSelectedNode();
  selected->AddChildNode(TreeNode{entity.GetComponent<TagComponent>().tag, entity.GetComponent<UUIDComponent>().uuid});
}

void EntityGUI::ReloadEntitySettings(Entity entity)
{
  settingsContainer->RemoveAllComponents();
  if(!entity)
    return;
  settingsContainer->AddComponent(guiAddComponent);

  settingsContainer->AddComponent(guiTagComponent);
  guiTagComponent->GetComponentByName<TextBox>("tag")->SetText(entity.GetComponent<TagComponent>().tag);

  std::vector<std::string> addComponents;

  if(entity.HasComponent<Transform2DComponent>())
  {
    guiTransform2D->AttachTo(settingsContainer, entity.GetComponent<Transform2DComponent>());
  }
  else
  {
    addComponents.emplace_back("Transform 2D Component");
  }

  if(entity.HasComponent<ColorComponent>())
  {
    settingsContainer->AddComponent(guiColorComponent);
    guiColorComponent->GetComponentByName<ColorPicker>("color")->SetColor(entity.GetComponent<ColorComponent>().color);
  }
  else
  {
    addComponents.emplace_back("Color component");
  }

  if(entity.HasComponent<SpriteComponent>())
  {
    guiSprite->AttachTo(settingsContainer, entity.GetComponent<SpriteComponent>());
  }
  else
  {
    addComponents.emplace_back("Sprite Component");
  }

  if(entity.HasComponent<Transform3DComponent>())
  {
    guiTransform3D->AttachTo(settingsContainer, entity.GetComponent<Transform3DComponent>());
  }

  if(entity.HasComponent<MeshComponent>())
  {
    settingsContainer->AddComponent(guiMeshComponent);
  }
  guiAddComponent->SetDropDownItems(addComponents);
  if(addComponents.empty())
  {
    settingsContainer->RemoveComponent(guiAddComponent);
  }
}

void EntityGUI::SelectEntity(Entity entity)
{
  if(entity)
  {
    TreeNode* treeNode = treeView->GetTreeNode(entity.GetComponent<UUIDComponent>().uuid);
    treeView->SelectTreeNode(treeNode);
  }
  else
  {
    treeView->SelectTreeNode(nullptr);
  }
  ReloadEntitySettings(entity);
}

void EntityGUI::GUITreeViewEntitySelected(TreeView* view, TreeNode* node, bool selected)
{
  Entity entity{entityManager->GetECS().get()};
  if(selected)
  {
    entity = entityManager->GetECS()->Find<UUIDComponent>(
      [node](EntityID entity, UUIDComponent& uuid)
      {
        return uuid.uuid == node->GetUUID();
      });
    ASSERT(entity, "Selected node does not exist in ECSManager %s", node->GetName());
    entityManager->SelectEntity(entity);
  }
}

void EntityGUI::GUIButtonCreateEntity2D(Component* component)
{
  entityManager->CreateEntity2D();
}

void EntityGUI::GUIButtonCreateEntity3D(Component* component)
{
  entityManager->CreateEntity3D();
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

void EntityGUI::GUIDropDownMenuAddComponent(Greet::Component* component, const std::string& oldLabel, const std::string& newLabel)
{
  if(entityManager->GetSelectedEntity())
  {
    if(newLabel == "Transform 2D Component")
      entityManager->GetSelectedEntity().AddComponent<Transform2DComponent>();
    else if(newLabel == "Color Component")
      entityManager->GetSelectedEntity().AddComponent<ColorComponent>();
    else if(newLabel == "Sprite Component")
      entityManager->GetSelectedEntity().AddComponent<SpriteComponent>();
    ReloadEntitySettings(entityManager->GetSelectedEntity());
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

void EntityGUI::GUITextBoxTagName(Greet::Component* component, const std::string& oldLabel, const std::string& newLabel)
{
  Entity selectedEntity = entityManager->GetSelectedEntity();
  if(selectedEntity)
  {
    ASSERT(selectedEntity.HasComponent<TagComponent>(), "Selected entity does not have a TagComponent");
    ASSERT(treeView->HasSelectedNode(), "No TreeView node is selected");

    selectedEntity.GetComponent<TagComponent>().tag = newLabel;
    treeView->GetSelectedNode()->SetName(newLabel);
  }
}

void EntityGUI::GUIColorPickerColor(Greet::Component* component, const Color& oldColor, const Color& newColor)
{
  Entity selectedEntity = entityManager->GetSelectedEntity();
  if(selectedEntity)
  {
    ASSERT(selectedEntity.HasComponent<ColorComponent>(), "Selected entity does not have a ColorComponent");
    selectedEntity.GetComponent<ColorComponent>().color = newColor.AsUInt();
  }
}
