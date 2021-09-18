#include "Layer.h"

#include <graphics/shaders/Shader.h>
#include <graphics/Renderable.h>
#include <graphics/renderers/BatchRenderer.h>
#include <graphics/renderers/Renderer2D.h>
#include <graphics/layers/Scene.h>

namespace Greet {
  Layer::Layer(const Ref<Renderer2D>& renderer, const Mat3& projectionMatrix)
    : renderer(renderer), projectionMatrix(projectionMatrix)
  {
    renderer->GetShader()->Enable();
    renderer->GetShader()->SetUniformMat3("uProjectionMatrix", projectionMatrix);
    renderer->GetShader()->SetUniformMat3("uViewMatrix", Mat3::Scale(1));
    renderer->GetShader()->Disable();
  }

  Layer::~Layer()
  {}

  void Layer::Add(const Ref<Renderable>& renderable)
  {
    renderables.push_back(renderable);
  }

  void Layer::Render() const
  {
    renderer->Begin();
    setUniforms();
    uint32_t size = renderables.size();
    for (uint32_t i = 0; i < size; i++)
    {
      renderables[i]->Begin(renderer);
      renderables[i]->Render(renderer);
      renderables[i]->End(renderer);
    }
    renderer->End();
    renderer->Flush();
  }

  void Layer::Update(float timeElapsed)
  {
    uint32_t size = renderables.size();
    for (uint32_t i = 0; i < size; i++)
      renderables[i]->Update(timeElapsed);
  }

  void Layer::SetProjectionMatrix(const Mat3& amProjectionMatrix)
  {
    projectionMatrix = amProjectionMatrix;
    renderer->GetShader()->Enable();
    renderer->GetShader()->SetUniformMat3("uProjectionMatrix", projectionMatrix);
    renderer->GetShader()->Disable();
  }
}
