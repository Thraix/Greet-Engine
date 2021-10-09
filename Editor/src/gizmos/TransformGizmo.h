#pragma once

#include <math/Vec2.h>
#include <math/Vec3.h>

namespace Greet
{
  class Camera3DComponent;
  class Event;
}

class TransformGizmo
{
  public:
    Greet::Vec3f position{0, 0, 0};
    Greet::Vec3f scale{1, 1, 1};
    Greet::Vec3f rotation{0, 0, 0};

    virtual void Render(const Greet::Camera3DComponent& camera) const = 0;
    virtual bool OnEvent(Greet::Event& event, const Greet::Camera3DComponent& cameraComponent) = 0;
};

