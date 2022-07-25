#pragma once

#include <graphics/gui/Component.h>
#include <graphics/SceneManager.h>

namespace Greet
{
  class SceneView : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(SceneView);

      SceneManager sceneManager;
    public:
      SceneView(const XMLObject& xmlObject, Component* parent);

      void Update(float timeElapsed) override;
      void Render(GUIRenderer* renderer) const override;
      void OnEvent(Event& event, const Vec2f& componentPos) override;
      void OnMeasured() override;

      const SceneManager& GetSceneManager() const;
      SceneManager& GetSceneManager();

      void Add2DScene(const Ref<Scene>& scene, const std::string& name);
      void Add3DScene(const Ref<Scene>& scene, const std::string& name);

      const Ref<Scene>& Remove2DScene(const std::string& name);
      const Ref<Scene>& Remove3DScene(const std::string& name);
      const Ref<Scene>& Get2DScene(const std::string& name) const;
      const Ref<Scene>& Get3DScene(const std::string& name) const;
  };
}

