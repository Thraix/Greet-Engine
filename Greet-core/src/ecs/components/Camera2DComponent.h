#pragma once

#include <event/ViewportEvent.h>
#include <graphics/shaders/Shader.h>
#include <math/Mat3.h>
#include <math/Vec2.h>
#include <utils/MetaFile.h>

namespace Greet
{

  struct Camera2DComponent {

    public:
      bool active;

    private:
      Mat3 projectionMatrix;
      Mat3 viewMatrix;
      Vec2f cameraPos;

    public:
      Camera2DComponent(const Mat3& viewMatrix, bool active);
      Camera2DComponent(const MetaFileClass& metaClass);

      const Mat3& GetViewMatrix() const;
      const Mat3& GetProjectionMatrix() const;
      void SetProjectionMatrix(const Mat3& amProjectionMatrix);
      void SetViewMatrix(const Mat3& amViewMatrix);
      void SetShaderUniforms(const Ref<Shader>& shader) const;
      void ViewportResize(ViewportResizeEvent& event);
  };
}
