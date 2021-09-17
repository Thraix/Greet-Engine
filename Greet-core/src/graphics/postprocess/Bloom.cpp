#include "Bloom.h"

#include <graphics/RenderCommand.h>
#include <graphics/shaders/ShaderFactory.h>
#include <input/Input.h>

namespace Greet
{
  Bloom::Bloom(uint32_t width, uint32_t height, int iterations)
  {
    thresholdBuffer = NewRef<Framebuffer>(width >> 1, height >> 1, true);
    for(int i = 0; i < iterations; i++)
    {
      blurBuffers.emplace_back(NewRef<Framebuffer>(width >> (i + 2), height >> (i + 2), true));
      blurBuffers.emplace_back(NewRef<Framebuffer>(width >> (i + 2), height >> (i + 2), true));
    }

    for(int i = 0; i < iterations; i++)
    {
      upsampleBuffers.emplace_back(NewRef<Framebuffer>(width >> (i + 1), height >> (i + 1), true));
    }

    additiveBuffer = NewRef<Framebuffer>(width, height, true);

    thresholdShader = ShaderFactory::ShaderThreshold();
    blurShader = ShaderFactory::ShaderBlur();
    additiveShader = ShaderFactory::ShaderAdditive();
    tonemappingShader = ShaderFactory::ShaderTonemapping();
    quadMesh = NewRef<Mesh>(MeshData(Pointer<Vec3f>{{-1, -1, 0}, {1, -1, 0}, {1, 1, 0}, {-1, 1, 0}}, Pointer<uint32_t>{0, 1, 2, 0, 2, 3}));
  }

  Bloom::~Bloom()
  {
  }

  void Bloom::Resize(uint32_t width, uint32_t height)
  {
    thresholdBuffer->Resize(width >> 1, height >> 1);
    for(int i = 0; i < blurBuffers.size() / 2; i++)
    {
      blurBuffers[i * 2]->Resize(width >> (i + 2), height >> (i + 2));
      blurBuffers[i * 2 + 1]->Resize(width>> (i + 2), height >> (i + 2));
    }

    for(int i = 0; i < blurBuffers.size() / 2; i++)
    {
      upsampleBuffers[i]->Resize(width >> (i + 1), height >> (i + 1));
    }
    additiveBuffer->Resize(width, height);
  }

  void Bloom::Render(const Ref<Texture2D>& texture, const Environment3DComponent& component)
  {
    quadMesh->Bind();

    thresholdBuffer->Bind();
    thresholdShader->Enable();
    thresholdShader->SetUniform1f("uThreshold", component.bloomThreshold);
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
    additiveShader->SetUniform1f("uWeightTexture1", 1.0f);
    additiveShader->SetUniform1f("uWeightTexture2", component.bloomFalloff);
    for(int i = upsampleBuffers.size() - 1; i >= 1; i--)
    {
      upsampleBuffers[i]->Bind();
      lastBuffer->GetColorTexture()->Enable(0);
      blurBuffers[i * 2 + 1]->GetColorTexture()->Enable(1);
      quadMesh->Render();
      upsampleBuffers[i]->Unbind();
      lastBuffer = upsampleBuffers[i].get();
    }

    upsampleBuffers[0]->Bind();
    lastBuffer->GetColorTexture()->Enable(0);
    thresholdBuffer->GetColorTexture()->Enable(1);
    quadMesh->Render();
    upsampleBuffers[0]->Unbind();

    additiveBuffer->Bind();
    additiveShader->SetUniform1f("uWeightTexture1", 1.0f);
    additiveShader->SetUniform1f("uWeightTexture2", 1.0f);
    texture->Enable(0);
    upsampleBuffers[0]->GetColorTexture()->Enable(1);
    quadMesh->Render();
    additiveShader->Disable();
    additiveBuffer->Unbind();

    tonemappingShader->Enable();
    tonemappingShader->SetUniform1i("uTexture", 0);
    additiveBuffer->GetColorTexture()->Enable(0);
    quadMesh->Render();
    quadMesh->Unbind();
    tonemappingShader->Disable();
  }
}
