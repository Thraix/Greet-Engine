#include "GUIScene.h"

#include <event/EventDispatcher.h>
#include <graphics/RenderCommand.h>
#include <graphics/gui/Frame.h>
#include <graphics/gui/GUIUtils.h>
#include <graphics/shaders/ShaderFactory.h>

namespace Greet {

  GUIScene::GUIScene()
    : renderer{new GUIRenderer()}, projectionMatrix(Mat3::OrthographicViewport())
  {
    focusQueue = nullptr;
    focused = nullptr;

    renderer->GetShader()->Enable();
    renderer->GetShader()->SetUniformMat3("projectionMatrix", projectionMatrix);
    renderer->GetShader()->Disable();
  }

  GUIScene::~GUIScene()
  {
    for(auto& frame : frames)
    {
      delete frame;
    }
    while(!addQueue.empty())
    {
      delete addQueue.front();
      addQueue.pop();
    }
    while(!removeQueue.empty())
    {
      delete removeQueue.front();
      removeQueue.pop();
    }
  }

  void GUIScene::OnEvent(Event& event)
  {
    Scene::OnEvent(event);
    if(EVENT_IS_CATEGORY(event, INPUT_EVENT))
    {
      if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
        OnMousePressEventHandler((MousePressEvent&)event);
      else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
        OnMouseMoveEventHandler((MouseMoveEvent&)event);
      else if(EVENT_IS_TYPE(event, EventType::MOUSE_RELEASE))
        OnMouseReleaseEventHandler((MouseReleaseEvent&)event);
      else if(focused)
      {
        focused->OnEventHandler(event, focused->GetPosition());
        event.AddFlag(EVENT_HANDLED);
      }
    }
  }

  void GUIScene::OnMousePressEventHandler(MousePressEvent& event)
  {
    Vec2f mousePos = ~projectionMatrix * event.GetPosition();
    MousePressEvent transformedEvent{mousePos, event.GetButton()};

    for (auto it = frames.rbegin(); it != frames.rend(); ++it)
    {
      if((*it)->IsMouseInside(transformedEvent.GetPosition() - (*it)->GetMargin().LeftTop() - (*it)->GetPosition()))
      {
        (*it)->OnEventHandler(transformedEvent, (*it)->GetPosition());
        frames.splice(frames.end(), frames, std::next(it).base());
        event.AddFlag(EVENT_HANDLED | transformedEvent.GetFlags());
        return;
      }
    }
    RequestFocus(nullptr);
    event.AddFlag(EVENT_UNFOCUSED);
  }

  void GUIScene::OnMouseMoveEventHandler(MouseMoveEvent& event)
  {
    Vec2f mousePos = ~projectionMatrix * event.GetPosition();
    Vec2f mouseDelta  = ~projectionMatrix * (event.GetPosition() + event.GetDeltaPosition()) - mousePos;
    MouseMoveEvent transformedEvent{mousePos, mouseDelta};

    if (focused && focused->UsingMouse())
    {
      focused->OnEventHandler(transformedEvent, focused->GetRealPosition());
      event.AddFlag(EVENT_HANDLED | transformedEvent.GetFlags());
      return;
    }

    for (auto it = frames.rbegin(); it != frames.rend(); ++it)
    {
      (*it)->OnEventHandler(transformedEvent, (*it)->GetPosition());
    }
  }

  void GUIScene::OnMouseReleaseEventHandler(MouseReleaseEvent& event)
  {
    Vec2f mousePos = ~projectionMatrix * event.GetPosition();
    MouseReleaseEvent transformedEvent{mousePos, event.GetButton()};
    if (focused)
    {
      focused->OnEventHandler(transformedEvent, focused->GetRealPosition());
      event.AddFlag(EVENT_HANDLED | transformedEvent.GetFlags());
    }
  }

  void GUIScene::ViewportResize(ViewportResizeEvent& event)
  {
    projectionMatrix = Mat3::OrthographicViewport();
    renderer->GetShader()->Enable();
    renderer->GetShader()->SetUniformMat3("projectionMatrix", projectionMatrix);
    renderer->GetShader()->Disable();
    for (auto it = frames.begin(); it != frames.end(); ++it)
    {
      (*it)->OnViewportResize(event.GetWidth(), event.GetHeight());
    }
  }

  void GUIScene::BeginRender() const
  {
    renderer->Begin();
  }

  void GUIScene::Render() const
  {
    BeginRender();
    for (auto it = frames.begin(); it != frames.end(); ++it)
    {
      (*it)->PreRender(renderer.get(), Vec2f(0,0));
      (*it)->RenderHandle(renderer.get());
      (*it)->PostRender(renderer.get());
    }
    EndRender();
  }

  void GUIScene::EndRender() const
  {
    renderer->End();
    renderer->Draw();
  }

  void GUIScene::Update(float timeElapsed)
  {
    while(!addQueue.empty())
    {
      Frame* frame = addQueue.front();
      frame->Measure(GetSize(), {1, 1});
      frame->SetGUIScene(this);
      frames.push_back(frame);
      frame->PostConstruction();
      addQueue.pop();
    }
    if(focusQueue.has_value())
      RequestFocus(focusQueue.value());
    focusQueue.reset();

    while(!removeQueue.empty())
    {
      Frame* frame = removeQueue.front();
      for (auto it = frames.begin(); it != frames.end();++it)
      {
        if ((*it) == frame)
        {
          frames.erase(it);
          break;
        }
      }
      removeQueue.pop();
    }
    for (auto it = frames.begin(); it != frames.end(); ++it)
    {
      if((*it)->remeasure)
      {
        (*it)->Measure(GetSize(), {1, 1});
      }
      (*it)->UpdateHandle(timeElapsed);
    }
  }

  void GUIScene::RequestFocusQueued(Component* component)
  {
    focusQueue = component;
  }

  // Could do checks if a popup is refusing to give request or something
  void GUIScene::RequestFocus(Component* component)
  {
    // Unfocus the currently focused component
    if(focused)
      focused->OnUnfocused();

    // Focus the requested one
    if(component && component != focused)
      component->OnFocused();

    // TODO: Maybe clean this code up.
    Component* unfocused = focused;
    Component* tmp = component;

    uint32_t unfocusedDepth = unfocused ? unfocused->GetComponentDepth() : 0;
    uint32_t focusedDepth = tmp ? tmp->GetComponentDepth() : 0;

    while(unfocusedDepth != focusedDepth)
    {
      if(unfocusedDepth > focusedDepth)
      {
        unfocused->ChildChangedFocus(false);
        unfocused = unfocused->GetParent();
        --unfocusedDepth;
      }
      else
      {
        tmp->ChildChangedFocus(true);
        tmp = tmp->GetParent();
        --focusedDepth;
      }
    }

    while(unfocused != tmp)
    {
      unfocused->ChildChangedFocus(false);
      unfocused = unfocused->GetParent();
      tmp->ChildChangedFocus(true);
      tmp = tmp->GetParent();
    }

    focused = component;
  }

  void GUIScene::AddFrameQueued(Frame* frame)
  {
    if (frame == nullptr)
    {
      Log::Warning("Cannot add nullptr to frames");

      return;
    }
    addQueue.push(frame);
  }

  void GUIScene::RemoveFrameQueued(const std::string& name)
  {
    for(auto it = frames.begin(); it!=frames.end();++it)
    {
      if((*it)->GetName() == name)
      {
        RemoveFrameQueued(*it);
        return;
      }
    }
    return;
  }

  void GUIScene::RemoveFrameQueued(Frame* frame)
  {
    removeQueue.push(frame);
  }

  Frame* GUIScene::GetFrame(const std::string& name)
  {
    for(auto it = frames.begin(); it!=frames.end();++it)
    {
      if((*it)->GetName() == name)
      {
        return (*it);
      }
    }
    return nullptr;
  }
  float GUIScene::GetWidth() const
  {
    return RenderCommand::GetViewportWidth();
  }
  float GUIScene::GetHeight() const
  {
    return RenderCommand::GetViewportHeight();
  }

  Vec2f GUIScene::GetSize() const
  {
    return {RenderCommand::GetViewportWidth(), RenderCommand::GetViewportHeight()};
  }
}
