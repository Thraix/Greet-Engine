#pragma once

#include <graphics/shaders/Shader.h>
#include <graphics/layers/Scene.h>
#include <graphics/renderers/GUIRenderer.h>
#include <event/Event.h>
#include <event/KeyEvent.h>
#include <event/MouseEvent.h>
#include <string>
#include <list>

namespace Greet {

  class Frame;
  class Component;

  class GUIScene : public Scene
  {
    private:
      Mat3 projectionMatrix;
      std::list<Frame*> frames;
      Component* m_focused;
      GUIRenderer* m_renderer;
      Ref<Shader> m_shader;

    public:
      GUIScene(GUIRenderer* renderer);
      GUIScene(GUIRenderer* renderer, const Ref<Shader>& shader);

      void OnEvent(Event& event);
      void OnMousePressEventHandler(MousePressEvent& event);
      void OnMouseMoveEventHandler(MouseMoveEvent& event);
      void OnMouseReleaseEventHandler(MouseReleaseEvent& event);
      void ViewportResize(ViewportResizeEvent& event);

      void PreRender();
      void Render() const;
      void PostRender();
      void Update(float timeElapsed);

      bool RequestFocus(Component* component);
      void AddFrame(Frame* frame);
      Frame* RemoveFrame(const std::string& name);
      Frame* RemoveFrame(Frame* frame);
      Frame* GetFrame(const std::string& name);

      float GetWidth() const;
      float GetHeight() const;
      Vec2 GetSize() const;

  };
}
