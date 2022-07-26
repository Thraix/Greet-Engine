#pragma once

#include <internal/App.h>

namespace Greet
{
  class EntityManager;
  class GUIScene;

  class Editor : public App
  {
    private:
      Ref<GUIScene> gui;
      Ref<EntityManager> entityManager;
    public:
      Editor();

      ~Editor();

      void Init() override;
      void Destruct() override;

      void Tick() override;
      void Update(float timeElapsed) override;
      void Render() override;
      void OnEvent(Event& event) override;
  };
}
