#include "Renderer2D.h"

#include <logging/Log.h>

namespace Greet {

  Renderer2D::Renderer2D(const Ref<Shader>& shader)
    : shader{shader}
  {
    m_transformationStack.push(Mat3::Identity());
    m_transformationBack = &m_transformationStack.top();
  }

  Renderer2D::~Renderer2D() {}
  void Renderer2D::Begin() {}
  void Renderer2D::End() {}

  void Renderer2D::PushMatrix(const Mat3 &matrix, bool override)
  {
    if (override)
      m_transformationStack.push(matrix);
    else
      m_transformationStack.push(*m_transformationBack * matrix);
    m_transformationBack = &m_transformationStack.top();

  }

  void Renderer2D::PopMatrix()
  {
    if (m_transformationStack.size() > 1)
      m_transformationStack.pop();
    else
      Log::Warning("Trying to pop the last matrix.");
    m_transformationBack = &m_transformationStack.top();
  }

  const Mat3& Renderer2D::GetMatrix() const
  {
    return *m_transformationBack;
  }

  const Ref<Shader>& Renderer2D::GetShader() const
  {
    return shader;
  }

  void Renderer2D::SetShader(const Ref<Shader>& aShader)
  {
    shader = aShader;
  }
}
