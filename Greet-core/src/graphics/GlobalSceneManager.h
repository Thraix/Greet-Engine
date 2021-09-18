#pragma once

#include <graphics/SceneManager.h>

namespace Greet {
  class GlobalSceneManager
  {
    DELETE_CONSTRUCTOR_DESTRUCTOR(GlobalSceneManager);

    private:
      static SceneManager* sceneManager;
    public:
      static void Create();
      static void Destroy();
      static SceneManager& GetSceneManager();
  };
}
