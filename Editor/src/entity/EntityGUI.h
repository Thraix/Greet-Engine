#pragma once

#include <ecs/Entity.h>
#include <ecs/components/Transform2DComponent.h>
#include <ecs/components/Transform3DComponent.h>
#include <graphics/gui/Container.h>
#include <graphics/gui/Frame.h>
#include <graphics/gui/TreeNode.h>
#include <graphics/gui/TreeView.h>
#include <graphics/gui/DropDownMenu.h>
#include <graphics/Color.h>

class EntityManager;
class GUITransform2D;
class GUITransform3D;
class GUISprite;
class GUIAnimation;

class EntityGUI
{
  private:
    EntityManager* entityManager;

    // GUI references
    Greet::TreeNode* treeNode;
    Greet::TreeView* treeView;
    Greet::Container* settingsContainer;

    // GUI Ecs components
    Greet::Ref<GUITransform2D> guiTransform2D;
    Greet::Ref<GUITransform3D> guiTransform3D;
    Greet::Ref<GUISprite> guiSprite;
    Greet::Ref<GUIAnimation> guiAnimation;
    Greet::Component* guiTagComponent;
    Greet::Component* guiMeshComponent;
    Greet::Component* guiColorComponent;

    // Add Component drop down menu
    Greet::DropDownMenu* guiAddComponent;

    // GUI data
    std::string meshType;
    std::string meshPath;

  public:
    EntityGUI(EntityManager* entityManager, Greet::Frame* frame);
    virtual ~EntityGUI();

    void UpdateSelectedTransform3D();
    void UpdateSelectedTransform2D();
    void CreateEntity(Greet::Entity entity);
    void SelectEntity(Greet::Entity entity);
    void ReloadEntitySettings(Greet::Entity entity);

  private:
    void GUITreeViewEntitySelected(Greet::TreeView* view, Greet::TreeNode* node, bool selected);
    void GUIButtonCreateEntity2D(Greet::Component* component);
    void GUIButtonCreateEntity3D(Greet::Component* component);
    void GUIDropDownMenuMeshType(Greet::Component* component, const std::string& oldLabel, const std::string& newLabel);
    void GUIDropDownMenuAddComponent(Greet::Component* component, const std::string& oldLabel, const std::string& newLabel);
    void GUITextBoxMeshPath(Greet::Component* component, const std::string& oldLabel, const std::string& newLabel);
    void GUITextBoxTagName(Greet::Component* component, const std::string& oldLabel, const std::string& newLabel);
    void GUIColorPickerColor(Greet::Component* component, const Greet::Color& oldColor, const Greet::Color& newColor);
};
