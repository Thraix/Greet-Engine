#pragma once

#include <graphics/cameras/Camera3D.h>
#include <math/Mat4.h>
#include <math/Vec3.h>

namespace Greet {

  class FPCamera3D : public Camera3D
  {
    public:
      Vec3f position;
      float yaw;
      float pitch;
      float roll;

      FPCamera3D(float fov, float near, float far, const Vec3f& position, const Vec3f& rotation)
        : Camera3D(fov, near, far), position(position), yaw(rotation.x), pitch(rotation.y), roll(rotation.z)
      {
        SetViewMatrix(Mat4::ViewMatrix(position, Vec3f(yaw,pitch, roll)));
      }

  };
}
