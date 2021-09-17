#pragma once

#include <common/Memory.h>
#include <ecs/components/Environment3DComponent.h>
#include <graphics/Framebuffer.h>
#include <graphics/models/Mesh.h>
#include <graphics/shaders/Shader.h>

#include <stdint.h>
#include <vector>

namespace Greet
{
  class Bloom final
  {
    private:
      Ref<Framebuffer> thresholdBuffer;
      std::vector<Ref<Framebuffer>> blurBuffers;
      std::vector<Ref<Framebuffer>> upsampleBuffers;
      Ref<Framebuffer> additiveBuffer;
      Ref<Shader> thresholdShader;
      Ref<Shader> blurShader;
      Ref<Shader> additiveShader;
      Ref<Shader> tonemappingShader;
      Ref<Mesh> quadMesh;

    public:
      Bloom(uint32_t width, uint32_t height, int iterations);
      ~Bloom();

      void Resize(uint32_t width, uint32_t height);

      void Render(const Ref<Texture2D>& texture, const Environment3DComponent& component);
  };
}
