#pragma once

#include <common/Memory.h>
#include <ecs/Entity.h>

namespace Greet {
  class Frame;
  class SceneView;
  class Mesh;

  class EntityGUI;
  class EntityScene;

  enum class NotifyOrigin
  {
    GUI, Scene
  };

  class EntityManager
  {
    DELETE_COPY_AND_MOVE(EntityManager);

    private:
      SceneView* sceneView;

      Ref<ECSManager> ecs;
      Entity selectedEntity;

      Ref<EntityGUI> gui;
      Ref<EntityScene> scene;

      int entityId = 0;

    public:
      EntityManager(Frame* frame);
      virtual ~EntityManager();

      void UpdateSelectedTransform3D(NotifyOrigin notifyOrigin);
      void UpdateSelectedTransform2D(NotifyOrigin notifyOrigin);
      void UpdateSelectedMesh(const Ref<Mesh>& mesh);
      void SelectEntity(Entity entity);
      void Serialize(const std::string& str);

      void CreateEntity3D();
      void CreateEntity2D();
      void DeleteEntity(Entity entity);
      Entity GetSelectedEntity() const;
      const Ref<ECSManager>& GetECS() const;

      const Ref<EntityGUI> GetEntityGUI() const;
      Ref<EntityGUI> GetEntityGUI();
  };
}
