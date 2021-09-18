#include "Bloom.h"

#include <graphics/RenderCommand.h>
#include <graphics/shaders/ShaderFactory.h>

namespace Greet
{
  Bloom::Bloom(uint32_t width, uint32_t height, int iterations)
  {
    float div = 0.25;
    thresholdBuffer = NewRef<Framebuffer>(width * 0.25, height * 0.25, true);
    for(int i = 0; i < iterations; i++)
    {
      blurBuffers.emplace_back(NewRef<Framebuffer>(width * div, height * div, true));
      blurBuffers.emplace_back(NewRef<Framebuffer>(width * div, height * div, true));
      div *= 0.5;
    }

    thresholdShader = ShaderFactory::ShaderThreshold();
    blurShader = ShaderFactory::ShaderBlur();
    additiveShader = ShaderFactory::ShaderAdditive();
    quadMesh = NewRef<Mesh>(MeshData(Pointer<Vec3f>{{-1, -1, 0}, {1, -1, 0}, {1, 1, 0}, {-1, 1, 0}}, Pointer<uint32_t>{0, 1, 2, 0, 2, 3}));
  }

  Bloom::~Bloom()
  {
  }

  void Bloom::Resize(uint32_t width, uint32_t height)
  {
    float div = 0.25;
    thresholdBuffer->Resize(width * 0.5, height * 0.5);
    float lastHeight = height * 0.5;
    for(int i = 0; i < blurBuffers.size() / 2; i++)
    {
      blurBuffers[i * 2]->Resize(width * div, lastHeight);
      blurBuffers[i * 2 + 1]->Resize(width * div, height * div);
      lastHeight = height * div;
      div *= 0.5;
    }
  }

  void Bloom::Render(const Ref<Texture2D>& texture)
  {
    quadMesh->Bind();

    thresholdBuffer->Bind();
    thresholdShader->Enable();
    thresholdShader->SetUniform1f("uThreshold", 0.80);
    thresholdShader->SetUniform1f("uIntensity", 0.5);
    texture->Enable(0);
    quadMesh->Render();
    thresholdBuffer->Unbind();

    blurShader->Enable();

    Framebuffer* lastBuffer = thresholdBuffer.get();

    for(int i = 0; i < blurBuffers.size() / 2; i++)
    {
      blurBuffers[i * 2]->Bind();
      blurShader->SetUniform2f("uSampleOffset", Vec2f(1.0f / (float)lastBuffer->GetColorTexture()->GetWidth(), 0));
      lastBuffer->GetColorTexture()->Enable(0);
      quadMesh->Render();
      blurBuffers[i * 2]->Unbind();

      blurBuffers[i * 2 + 1]->Bind();
      blurShader->SetUniform2f("uSampleOffset", Vec2f(0, 1.0f / (float)lastBuffer->GetColorTexture()->GetHeight()));
      blurBuffers[i * 2]->GetColorTexture()->Enable(0);
      quadMesh->Render();
      blurBuffers[i * 2 + 1]->Unbind();

      lastBuffer = blurBuffers[i * 2 + 1].get();
    }

    additiveShader->Enable();
    additiveShader->SetUniform1i("uTexture1", 0);
    additiveShader->SetUniform1i("uTexture2", 1);
    texture->Enable(0);
    lastBuffer->GetColorTexture()->Enable(1);
    quadMesh->Render();
    quadMesh->Unbind();
    additiveShader->Disable();
  }
}
