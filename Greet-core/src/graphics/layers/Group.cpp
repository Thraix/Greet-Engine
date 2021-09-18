#include "Group.h"

namespace Greet {

  Group::Group(const Vec2f& position)
    : m_transformationMatrix(Mat3::Translate(position))
  {
    enable = true;
  }

  Group::Group(const Mat3& transform)
    :m_transformationMatrix(transform)
  {
    enable = true;
  }

  Group::~Group()
  {
    uint32_t size = m_renderables.size();
    for (uint32_t i = 0; i < size; i++)
      delete m_renderables[i];
  }

  void Group::Add(Renderable* renderable)
  {
    m_renderables.push_back(renderable);
  }

  void Group::Remove(Renderable* renderable)
  {
    for (auto it = m_renderables.begin();it != m_renderables.end();it++)
    {
      if (*it == renderable)
      {
        m_renderables.erase(it);
        return;
      }
    }
  }

  bool Group::Update(float timeElapsed)
  {
    bool update = false;
    if (enable)
      for (uint32_t i = 0; i < m_renderables.size(); i++)
        update |= m_renderables[i]->Update(timeElapsed);
    return update;
  }

  void Group::Begin(const Ref<Renderer2D>& renderer) const
  {
    if (render)
      renderer->PushMatrix(m_transformationMatrix);
  }

  void Group::Render(const Ref<Renderer2D>& renderer) const
  {
    if (render)
      for (uint32_t i = 0; i < m_renderables.size(); i++)
      {
        m_renderables[i]->Begin(renderer);
        m_renderables[i]->Render(renderer);
        m_renderables[i]->End(renderer);
      }
  }

  void Group::End(const Ref<Renderer2D>& renderer) const
  {
    if (render)
      renderer->PopMatrix();
  }

}
