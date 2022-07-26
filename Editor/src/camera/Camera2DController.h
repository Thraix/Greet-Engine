#pragma once

#include <math/Vec2.h>
#include <ecs/Entity.h>
#include <event/Event.h>

namespace Greet
{
  class Camera2DController
  {
    struct Controller
    {
      Vec2f pressPos;
      Vec2f camPressPos;
      bool isMoving = false;
      bool mouseMiddleDown = false;
      bool mouseRightDown = false;
      bool keyShiftDown = false;
    };

    private:
      Controller controller;
    public:
      Entity entity{nullptr};

    public:
      Camera2DController(ECSManager* manager);
      virtual ~Camera2DController();

      void OnCreate();
      void OnEvent(Event& event);
  };
}
