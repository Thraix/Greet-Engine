#pragma once

#include <common/Memory.h>
#include <graphics/layers/Scene.h>

#include <vector>

namespace Greet {
  class SceneManager
  {
    DELETE_COPY_AND_MOVE(SceneManager);
    typedef std::pair<std::string, Ref<Scene>> SceneElement;

    private:
        // they are the same but 3d scenes are always rendered first
      std::vector<SceneElement> m_scenes2d;
      std::vector<SceneElement> m_scenes3d;
      Ref<Scene> focusedScene;

    public:
      SceneManager();
      virtual ~SceneManager();
      void Add2DScene(const Ref<Scene>& scene, const std::string& name);
      void Add3DScene(const Ref<Scene>& scene, const std::string& name);

      const Ref<Scene>& Remove2DScene(const std::string& name);
      const Ref<Scene>& Remove3DScene(const std::string& name);
      const Ref<Scene>& Get2DScene(const std::string& name) const;
      const Ref<Scene>& Get3DScene(const std::string& name) const;
      void Render() const;
      void Update(float timeElapsed);
      void OnEvent(Event& event);
  };
}
