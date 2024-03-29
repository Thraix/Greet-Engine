#pragma once

#include <ecs/ECSManager.h>
#include <ecs/Entity.h>
#include <graphics/layers/Scene.h>
#include <graphics/renderers/BatchRenderer.h>
#include <graphics/Framebuffer.h>
#include <graphics/postprocess/Bloom.h>
#include <utils/MetaFile.h>

namespace Greet
{
  class ECSScene : public Scene
  {
    protected:
      Ref<ECSManager> manager;
      Ref<BatchRenderer> renderer2d;
      Ref<Framebuffer> framebuffer;
      Ref<Bloom> bloom;

      Environment3DComponent defaultEnv{};

    public:
      ECSScene();
      ECSScene(const Ref<ECSManager>& ecsManager, const std::string& scenePath);
      ECSScene(const Ref<ECSManager>& ecsManager);
      ECSScene(const std::string& scenePath);
      virtual ~ECSScene();

      void LoadEntity(const MetaFile& meta, Entity entity, int depth = 0);
      void Serialize(const std::string& path);

      Entity AddEntity(const std::string& tag);
      void RemoveEntity(const Entity& entity);
      void RemoveEntity(EntityID entity);

      virtual void Render() const override;
      virtual void Render2D() const;

      virtual void Render3DScene() const;
      virtual void Render3DBegin(const Camera3DComponent& cam, const Environment3DComponent& env) const;
      virtual void Render3D(const Camera3DComponent& cam, const Environment3DComponent& env) const;
      virtual void Render3DEnd(const Camera3DComponent& cam, const Environment3DComponent& env) const;

      virtual void Update(float timeElapsed) override;
      virtual void OnEvent(Event& event) override;
      const ECSManager* GetManager() const { return manager.get(); }
      ECSManager* GetManager() { return manager.get(); }

      virtual Entity GetCamera2DEntity() const;
      virtual Entity GetCamera3DEntity() const;
      virtual Entity GetEnvironment2DEntity() const;
      virtual Entity GetEnvironment3DEntity() const;

    private:

      // Loads external components not handled by the engine.
      // Gives the developer more control over what components can be used
      virtual void LoadExtComponents(Entity& entity, const MetaFile& metaClass) {}
  };
}
