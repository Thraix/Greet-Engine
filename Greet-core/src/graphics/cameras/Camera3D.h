#pragma once

#include <event/Event.h>
#include <event/ViewportEvent.h>
#include <graphics/shaders/Shader.h>
#include <math/Line.h>
#include <math/Mat4.h>
#include <math/Vec2.h>
#include <math/Vec3.h>

// TODO: Move implementation to cpp file

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
      Camera3D(float fov, float near, float far) :
        projectionMatrix{Mat4::PerspectiveViewport(fov, near, far)},
        fov{fov}, near{near}, far{far}
      {}

      const Mat4& GetViewMatrix() const { return viewMatrix;}
      const Mat4& GetProjectionMatrix() const { return projectionMatrix; }

      void SetProjectionMatrix(const Mat4& amProjectionMatrix)
      {
        projectionMatrix = amProjectionMatrix;
        invPVMatrix = ~(projectionMatrix * viewMatrix);
      }

      void SetViewMatrix(const Mat4& amViewMatrix)
      {
        Mat4 invMatrix = ~amViewMatrix;
        cameraPos = Vec3f{invMatrix.columns[3]};
        viewMatrix = amViewMatrix;
        invPVMatrix = ~(projectionMatrix * viewMatrix);
      }

      void SetShaderUniforms(const Ref<Shader>& shader) const
      {
        shader->Enable();
        shader->SetUniform3f("uCameraPos", cameraPos);
        shader->SetUniformMat4("uViewMatrix", viewMatrix);
        shader->SetUniformMat4("uProjectionMatrix", projectionMatrix);
        shader->SetUniformMat4("uInvPVMatrix", invPVMatrix);
      }

      virtual void Update(float timeElapsed) {};
      virtual void ViewportResize(ViewportResizeEvent& event)
      {
        projectionMatrix = Mat4::Perspective(event.GetAspect(), fov, near, far);
      }

      virtual void OnEvent(Event& event) {};

      Vec3f GetWorldToScreenCoordinate(const Vec3f& coordinate) const
      {
        return projectionMatrix * (viewMatrix * coordinate);
      }

      Line GetScreenToWorldCoordinate(const Vec2f& screenPos) const
      {
        Line line;

        line.pos = invPVMatrix * Vec3f(screenPos.x, screenPos.y, -1.0);
        Vec3f far = invPVMatrix * Vec3f(screenPos.x, screenPos.y, 1.0);
        line.dir = far - line.pos;
        return line;
      }
  };
}
