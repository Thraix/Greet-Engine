#pragma once

#include <graphics/cameras/Camera3D.h>
#include <graphics/shaders/Shader.h>

namespace Greet{

  class Renderer3D
  {
    public:
      Renderer3D()
      {}

      virtual ~Renderer3D()
      {}

      virtual void Update(float timeElapsed) {}
      void BindMatrices(const Ref<Shader>& shader, const Ref<Camera3D>& camera, bool shouldBindShader = false) const;

      virtual void Begin(const Ref<Camera3D>& camera) {}
      virtual void Render(const Ref<Camera3D>& camera) const {}
      virtual void End(const Ref<Camera3D>& camera) {}
  };
}
