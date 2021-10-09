#pragma once

#include <ecs/Entity.h>
#include <ecs/components/Transform3DComponent.h>
#include <graphics/gui/Container.h>
#include <graphics/gui/Frame.h>
#include <graphics/gui/TreeNode.h>
#include <graphics/gui/TreeView.h>

class EntityManager;
class GUITransform3D;

class EntityGUI
{
  private:
    EntityManager* entityManager;

    // GUI references
    Greet::TreeNode* treeNode;
    Greet::TreeView* treeView;
    Greet::Container* settingsContainer;

    // GUI Ecs components
    Greet::Ref<GUITransform3D> guiTransform3D;
    Greet::Component* guiTransform2DComponent;

  public:
    EntityGUI(EntityManager* entityManager, Greet::Frame* frame);

    void UpdateSelectedTransform3D();
    void CreateEntity(const std::string& name);
    void SelectEntity(Greet::Entity entity);

  private:
    void GUITreeViewEntitySelected(Greet::TreeView* view, Greet::TreeNode* node, bool selected);
    void GUIButtonCreateEntity(Greet::Component* component);
};
