#include "SceneView.h"

#include <graphics/RenderCommand.h>
#include <input/Input.h>
#include <event/ViewportEvent.h>

namespace Greet
{

  REGISTER_COMPONENT_DEFINITION(SceneView);

  SceneView::SceneView(const XMLObject& xmlObject, Component* parent)
    : Component(xmlObject, parent)
  {}

  void SceneView::Update(float timeElapsed)
  {
    Vec2 translatedPos = GetRealPosition();
    RenderCommand::PushViewportStack(GetRealPosition(), GetContentSize());

    sceneManager.Update(timeElapsed);

    RenderCommand::PopViewportStack();
  }

  void SceneView::Render(GUIRenderer* renderer) const
  {
    guiScene->EndRender();
    RenderCommand::PushViewportStack(GetRealPosition(), GetContentSize());

    sceneManager.Render();

    RenderCommand::PopViewportStack();
    guiScene->BeginRender();
  }

  void SceneView::OnEvent(Event& event, const Vec2f& componentPos)
  {
    RenderCommand::PushViewportStack(componentPos, GetContentSize());
    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      guiScene->RequestFocusQueued(this);
      MousePressEvent transformedEvent{Input::GetMousePos(), static_cast<MousePressEvent&>(event).GetButton()};
      sceneManager.OnEvent(transformedEvent);
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      MouseMoveEvent transformedEvent{Input::GetMousePos(), Input::GetMousePosDelta()};
      sceneManager.OnEvent(transformedEvent);
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_RELEASE))
    {
      MouseReleaseEvent transformedEvent{Input::GetMousePos(), static_cast<MouseReleaseEvent&>(event).GetButton()};
      sceneManager.OnEvent(transformedEvent);
    }
    else
      sceneManager.OnEvent(event);
    RenderCommand::PopViewportStack();
  }

  void SceneView::OnMeasured()
  {
    if(width.size <= 0 || height.size <= 0)
      return;
    Vec2 realPos = GetRealPosition();
    RenderCommand::PushViewportStack(realPos, GetContentSize());
    ViewportResizeEvent event{realPos, GetContentSize()};
    sceneManager.OnEvent(event);
    RenderCommand::PopViewportStack();
  }

  SceneManager& SceneView::GetSceneManager()
  {
    return sceneManager;
  }

  void SceneView::Add2DScene(const Ref<Scene>& scene, const std::string& name)
  {
    sceneManager.Add2DScene(scene, name);
    Vec2 realPos = GetRealPosition();
    RenderCommand::PushViewportStack(realPos, GetContentSize());
    ViewportResizeEvent event{realPos, GetContentSize()};
    scene->OnEvent(event);
    RenderCommand::PopViewportStack();
  }

  void SceneView::Add3DScene(const Ref<Scene>& scene, const std::string& name)
  {
    sceneManager.Add3DScene(scene, name);
    Vec2 realPos = GetRealPosition();
    RenderCommand::PushViewportStack(realPos, GetContentSize());
    ViewportResizeEvent event{realPos, GetContentSize()};
    scene->OnEvent(event);
    RenderCommand::PopViewportStack();
  }

  const Ref<Scene>& SceneView::Remove2DScene(const std::string& name)
  {
    return sceneManager.Remove2DScene(name);
  }

  const Ref<Scene>& SceneView::Remove3DScene(const std::string& name)
  {
    return sceneManager.Remove3DScene(name);
  }

  const Ref<Scene>& SceneView::Get2DScene(const std::string& name) const
  {
    return sceneManager.Get2DScene(name);
  }

  const Ref<Scene>& SceneView::Get3DScene(const std::string& name) const
  {
    return sceneManager.Get3DScene(name);
  }
}
