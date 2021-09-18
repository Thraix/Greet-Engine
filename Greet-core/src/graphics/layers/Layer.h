#pragma once

#include <graphics/shaders/Shader.h>
#include <graphics/Renderable.h>
#include <graphics/renderers/Renderer2D.h>
#include <graphics/layers/Scene.h>

namespace Greet {

  class Layer : public Scene
  {
    protected:
      Ref<Renderer2D> renderer;
      std::vector<Ref<Renderable>> renderables;

      Mat3 projectionMatrix;
    public:
      Layer(const Ref<Renderer2D>& renderer, const Mat3& projectionMatrix);

      virtual ~Layer();

      virtual void Add(const Ref<Renderable>& renderable);
      virtual void Render() const override;
      virtual void Update(float timeElapsed) override;

      void SetProjectionMatrix(const Mat3& amProjectionMatrix);
      virtual void ViewportResize(float x, float y, float width, float height) {}
      virtual void ViewportResize(ViewportResizeEvent& event) override {}
      const Mat3& GetProjectionMatrix() const { return projectionMatrix; };
      inline const Ref<Shader>& GetShader() const { return renderer->GetShader(); }
      inline uint32_t Size() const { return renderables.size(); }

    protected:
      virtual void setUniforms() const {}
  };
}
