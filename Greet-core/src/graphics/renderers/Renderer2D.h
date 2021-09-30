#pragma once

#include <graphics/shaders/Shader.h>

#include <stack>

namespace Greet{

  class Renderable2D;

  class Renderer2D
  {
    protected:
      std::stack<Mat3> m_transformationStack;
      const Mat3* m_transformationBack;
      Ref<Shader> shader;
    public:
      Renderer2D(const Ref<Shader>& shader);

      virtual ~Renderer2D();
      virtual void Begin();
      virtual void Flush() = 0;
      virtual void End();

      virtual void Draw(const Renderable2D& renderable) = 0;

      void PushMatrix(const Mat3 &matrix, bool override = false);
      void PopMatrix();

      const Mat3& GetMatrix() const;

      const Ref<Shader>& GetShader() const;
      virtual void SetShader(const Ref<Shader>& aShader);
  };
}
