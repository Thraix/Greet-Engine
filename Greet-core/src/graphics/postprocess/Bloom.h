#pragma once

#include <graphics/Framebuffer.h>
#include <graphics/shaders/Shader.h>
#include <graphics/models/Mesh.h>

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
      Bloom(uint width, uint height, int iterations);
      ~Bloom();

      void Resize(uint width, uint height);

      void Render(const Ref<Texture2D>& texture);
  };
}
