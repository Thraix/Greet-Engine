#pragma once

#include <event/ViewportEvent.h>
#include <graphics/shaders/Shader.h>
#include <math/Line.h>
#include <math/Mat4.h>
#include <math/Vec3.h>
#include <utils/MetaFile.h>

namespace Greet
{

  struct Camera3DComponent {

    public:
      bool active;

    private:
      float fov;
      float near;
      float far;
      Vec3f cameraPos;
      Mat4 projectionMatrix;
      Mat4 viewMatrix;
      Mat4 invPVMatrix;

    public:
      Camera3DComponent(const Mat4& viewMatrix, float fov, float near, float far, bool active);
      Camera3DComponent(const MetaFileClass& metaClass);

      const Vec3f& GetPosition() const;
      const Mat4& GetViewMatrix() const;
      const Mat4& GetProjectionMatrix() const;
      const Mat4& GetInvPVMatrix() const;
      void SetProjectionMatrix(const Mat4& amProjectionMatrix);
      void SetViewMatrix(const Mat4& amViewMatrix);
      void SetShaderUniforms(const Ref<Shader>& shader) const;
      void ViewportResize(ViewportResizeEvent& event);
      Vec3f GetWorldToScreenCoordinate(const Vec3f& coordinate) const;
      Vec3f GetWorldToScreenCoordinate(const Mat4& transformation, const Vec3f& coordinate) const;
      Line GetScreenToWorldCoordinate(const Vec2f& screenPos) const;
      Line GetScreenToWorldCoordinate(const Mat4& invTransformation, const Vec2f& screenPos) const;
  };
}
