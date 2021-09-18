#pragma once

#include <graphics/Framebuffer.h>
#include <graphics/shaders/Shader.h>
#include <graphics/models/Mesh.h>
#include <common/Memory.h>

#include <stdint.h>
#include <vector>

namespace Greet
{
  class Bloom final
  {
    private:
      Ref<Framebuffer> thresholdBuffer;
      std::vector<Ref<Framebuffer>> blurBuffers;
      Ref<Shader> thresholdShader;
      Ref<Shader> blurShader;
      Ref<Shader> additiveShader;
      Ref<Mesh> quadMesh;

    public:
      Bloom(uint32_t width, uint32_t height, int iterations);
      ~Bloom();

      void Resize(uint32_t width, uint32_t height);

      void Render(const Ref<Texture2D>& texture);
  };
}
