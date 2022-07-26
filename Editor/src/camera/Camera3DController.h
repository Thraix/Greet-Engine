#pragma once

#include <ecs/Entity.h>
#include <event/Event.h>
#include <math/Mat4.h>

namespace Greet
{
  class Camera3DController
  {
    struct Controller
    {
      bool mouseMiddleDown = false;
      bool mouseRightDown = false;
      bool keyShiftDown = false;
    };

    private:
      Controller controller;
      Vec3f pos;
      Vec3f rot;
      float distance;

      Vec3f vel;
      Vec3f rotVel;

      Mat4 pressInvPVMatrix;
      Vec3f pressPlanePos;
      Vec3f pressLinePos;
      Vec3f pressPos;

    public:
      Entity entity{nullptr};

    public:
      Camera3DController(ECSManager* ecs, const Vec3f& pos = {}, const Vec3f& rot = {});

      void OnUpdate(float timeElapsed);
      void OnEvent(Event& event);

      Mat4 CalculateViewMatrix(const Vec3f& pos, const Vec3f& rot, float distance);
  };
}
