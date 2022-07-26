#pragma once

#include <common/Memory.h>
#include <ecs/Entity.h>
#include <graphics/Color.h>

namespace Greet
{
  class Component;
  class Container;
  class DropDownMenu;
  class Frame;
  class TextBox;
  class TreeNode;
  class TreeView;

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
      TreeNode* treeNode;
      TreeView* treeView;
      Container* settingsContainer;

      // GUI Ecs components
      Ref<GUITransform2D> guiTransform2D;
      Ref<GUITransform3D> guiTransform3D;
      Ref<GUISprite> guiSprite;
      Ref<GUIAnimation> guiAnimation;
      Component* guiTagComponent;
      Component* guiMeshComponent;
      Component* guiColorComponent;

      // Add Component drop down menu
      DropDownMenu* guiAddComponent;

      // GUI data
      std::string meshType;
      std::string meshPath;

    public:
      EntityGUI(EntityManager* entityManager, Frame* frame);
      virtual ~EntityGUI();

      void UpdateSelectedTransform3D();
      void UpdateSelectedTransform2D();
      void CreateEntity(Entity entity);
      void SelectEntity(Entity entity);
      void ReloadEntitySettings(Entity entity);

    private:
      void GUITreeViewEntitySelected(TreeView* view, TreeNode* node, bool selected);
      void GUIButtonCreateEntity2D(Component* component);
      void GUIButtonCreateEntity3D(Component* component);
      void GUIDropDownMenuMeshType(Component* component, const std::string& oldLabel, const std::string& newLabel);
      void GUIDropDownMenuAddComponent(Component* component, const std::string& oldLabel, const std::string& newLabel);
      void GUITextBoxMeshPath(Component* component, const std::string& oldLabel, const std::string& newLabel);
      void GUITextBoxTagName(Component* component, const std::string& oldLabel, const std::string& newLabel);
      void GUIColorPickerColor(Component* component, const Color& oldColor, const Color& newColor);
  };
}
