#pragma once

#include "EditorScene.h"

#include <ecs/Entity.h>
#include <graphics/gui/Container.h>
#include <graphics/gui/Frame.h>
#include <graphics/gui/TreeNode.h>
#include <graphics/gui/TreeView.h>

class EntityManager
{
  private:
    // GUI references
    Greet::TreeNode* sceneTree;
    Greet::TreeView* sceneTreeView;
    Greet::Container* settingsContainer;
    Greet::SceneView* sceneView;

    Greet::Ref<Greet::ECSManager> ecs;

    Greet::Entity selectedEntity;

    // Scene
    Greet::Ref<EditorScene> scene;

  public:
    EntityManager(Greet::Frame* frame);
    virtual ~EntityManager();

    DELETE_COPY_AND_MOVE(EntityManager);

    void SelectEntity(Greet::Entity entity);

    void CreateEntity();

    Greet::Entity GetSelectedEntity() const;

    const Greet::Ref<Greet::ECSManager>& GetECS() const;
};
