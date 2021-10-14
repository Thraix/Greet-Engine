#pragma once

#include <common/Memory.h>
#include <ecs/Entity.h>

namespace Greet {
  class ECSManager;
  class Frame;
  class SceneView;
  class Mesh;
}

enum class NotifyOrigin
{
  GUI, Scene
};

class EntityGUI;
class EntityScene;

class EntityManager
{
  private:
    Greet::SceneView* sceneView;

    Greet::Ref<Greet::ECSManager> ecs;
    Greet::Entity selectedEntity;

    Greet::Ref<EntityScene> scene;
    Greet::Ref<EntityGUI> gui;

  public:
    EntityManager(Greet::Frame* frame);
    virtual ~EntityManager();

    DELETE_COPY_AND_MOVE(EntityManager);

    void UpdateSelectedTransform3D(NotifyOrigin notifyOrigin);
    void UpdateSelectedMesh(const Greet::Ref<Greet::Mesh>& mesh);
    void SelectEntity(Greet::Entity entity);

    void CreateEntity();
    Greet::Entity GetSelectedEntity() const;
    const Greet::Ref<Greet::ECSManager>& GetECS() const;
};
