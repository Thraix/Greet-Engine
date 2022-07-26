#pragma once

#include <math/Vec2.h>
#include <math/Vec3.h>

namespace Greet
{
  class Camera3DComponent;
  class Event;

  class TransformGizmo
  {
    public:
      Vec3f position{0, 0, 0};
      Vec3f scale{1, 1, 1};
      Vec3f rotation{0, 0, 0};

      virtual void Render(const Camera3DComponent& camera) const = 0;
      virtual bool OnEvent(Event& event, const Camera3DComponent& cameraComponent) = 0;
  };
}
