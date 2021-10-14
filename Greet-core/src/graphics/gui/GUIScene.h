#pragma once

#include <event/Event.h>
#include <event/KeyEvent.h>
#include <event/MouseEvent.h>
#include <graphics/layers/Scene.h>
#include <graphics/renderers/GUIRenderer.h>

#include <list>
#include <optional>
#include <queue>

namespace Greet {

  class Frame;
  class Component;

  class GUIScene : public Scene
  {
    private:
      Mat3 projectionMatrix;
      std::list<Frame*> frames;
      Component* focused;
      Ref<GUIRenderer> renderer;

      std::queue<Frame*> addQueue;
      std::queue<Frame*> removeQueue;
      std::optional<Component*> focusQueue;

    public:
      GUIScene();
      virtual ~GUIScene();

      void OnEvent(Event& event) override;
      void OnMousePressEventHandler(MousePressEvent& event);
      void OnMouseMoveEventHandler(MouseMoveEvent& event);
      void OnMouseReleaseEventHandler(MouseReleaseEvent& event);
      void ViewportResize(ViewportResizeEvent& event) override;

      void BeginRender() const;
      void Render() const override;
      void EndRender() const;
      void Update(float timeElapsed) override;

      void RequestFocusQueued(Component* component);
      void AddFrameQueued(Frame* frame);
      void RemoveFrameQueued(const std::string& name);
      void RemoveFrameQueued(Frame* frame);
      Frame* GetFrame(const std::string& name);

      float GetWidth() const;
      float GetHeight() const;
      Vec2f GetSize() const;
    private:
      void RequestFocus(Component* component);

  };
}
