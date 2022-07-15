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
      Vec2f position;
      Vec2f scale;
      float rotation;

    private:
      Mat3 projectionMatrix;

    public:
      Camera2DComponent(const Vec2f& pos, const Vec2f& scale, float rotation, bool active);
      Camera2DComponent(const MetaFileClass& metaClass);

      Mat3 GetViewMatrix() const;
      const Mat3& GetProjectionMatrix() const;
      Mat3 GetPVMatrix() const;
      Mat3 GetInversePVMatrix() const;
      void SetProjectionMatrix(const Mat3& amProjectionMatrix);
      void SetShaderUniforms(const Ref<Shader>& shader) const;
      void ViewportResize(ViewportResizeEvent& event);

      friend MetaFile& operator<<(MetaFile& metaFile, const Camera2DComponent& component);
  };
}
