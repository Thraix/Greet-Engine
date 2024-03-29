#pragma once

#include <event/Event.h>
#include <event/ViewportEvent.h>
#include <graphics/shaders/Shader.h>
#include <math/Line.h>
#include <math/Mat4.h>
#include <math/Vec2.h>
#include <math/Vec3.h>

namespace Greet {
  class Camera3D {

    private:
      float fov;
      float near;
      float far;
      Vec3f cameraPos;
      Mat4 projectionMatrix;
      Mat4 viewMatrix;
      Mat4 invPVMatrix;

    public:
      Camera3D(float fov, float near, float far);

      const Mat4& GetViewMatrix() const;
      const Mat4& GetProjectionMatrix() const;
      void SetProjectionMatrix(const Mat4& amProjectionMatrix);
      void SetViewMatrix(const Mat4& amViewMatrix);
      void SetShaderUniforms(const Ref<Shader>& shader) const;

      virtual void Update(float timeElapsed);
      virtual void ViewportResize(ViewportResizeEvent& event);
      virtual void OnEvent(Event& event);

      Vec3f GetWorldToScreenCoordinate(const Vec3f& coordinate) const;
      Line GetScreenToWorldCoordinate(const Vec2f& screenPos) const;
  };
}
