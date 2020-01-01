#pragma once

#include <graphics/shaders/Shader.h>
#include <graphics/Renderable.h>
#include <graphics/renderers/BatchRenderer.h>
#include <graphics/renderers/Renderer2D.h>
#include <graphics/layers/Scene.h>

namespace Greet {

  class Layer : public Scene
  {
    protected:
      Renderer2D* m_renderer;
      std::vector<Renderable*> m_renderables;
      Ref<Shader> m_shader;

      Mat3 m_projectionMatrix;
    public:
      Layer(Renderer2D* renderer, const Ref<Shader>& shader, Mat3 projectionMatrix)
        : m_renderer(renderer), m_shader(shader), m_projectionMatrix(projectionMatrix)
      {
        int texIDs[32];
        for (int i = 0; i < 32; i++)
        {
          texIDs[i] = i;
        }
        m_shader->Enable();
        m_shader->SetUniformMat3("projectionMatrix", m_projectionMatrix);
        m_shader->SetUniform1iv("textures", 32, texIDs);
        m_shader->Disable();
      }

      virtual ~Layer()
      {
        delete m_renderer;

        for (uint i = 0; i < m_renderables.size(); i++)
          delete m_renderables[i];
      }

      virtual void Add(Renderable* renderable)
      {
        m_renderables.push_back(renderable);
      }

      virtual void PreRender() override
      {
        m_shader->Enable();
        setUniforms();
        m_renderer->Begin();
      }

      virtual void Render() const override
      {
        uint size = m_renderables.size();
        for (uint i = 0; i < size; i++)
        {
          m_renderables[i]->Begin(m_renderer);
          m_renderables[i]->Submit(m_renderer);
          m_renderables[i]->End(m_renderer);
        }
      }

      virtual void PostRender() override
      {
        m_renderer->End();
        m_renderer->Flush();
        m_shader->Disable();
      }

      virtual void Update(float timeElapsed) override
      {
        uint size = m_renderables.size();
        for (uint i = 0; i < size; i++)
          m_renderables[i]->Update(timeElapsed);
      }

      void SetProjectionMatrix(Mat3 projectionMatrix)
      {
        m_projectionMatrix = projectionMatrix;
        m_shader->Enable();
        m_shader->SetUniformMat3("projectionMatrix", m_projectionMatrix);
        m_shader->Disable();
      }

      virtual void ViewportResize(float x, float y, float width, float height) {}
      virtual void ViewportResize(ViewportResizeEvent& event) override {}
      Mat3 GetProjectionMatrix() const { return m_projectionMatrix; };
      inline const Ref<Shader>& GetShader() const { return m_shader; }
      inline uint Size() const { return m_renderables.size(); }


    protected:
      virtual void setUniforms() const {}
  };
}
