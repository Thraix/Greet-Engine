#include "Camera2DController.h"

#include "../entity/EntityScene.h"

#include <ecs/components/Camera2DComponent.h>
#include <ecs/components/Environment2DComponent.h>
#include <ecs/components/TagComponent.h>
#include <ecs/components/UUIDComponent.h>
#include <event/KeyEvent.h>
#include <event/MouseEvent.h>
#include <graphics/shaders/ShaderFactory.h>
#include <input/Input.h>
#include <logging/Log.h>

namespace Greet
{
  Camera2DController::Camera2DController(ECSManager* manager) {
    entity = Entity{manager, manager->CreateEntity()};
    entity.AddComponent<UUIDComponent>(UUID{});
    entity.AddComponent<TagComponent>("EditorCamera2D");
    entity.AddComponent<Camera2DComponent>(Vec2f{0, 0}, Vec2f{1, 1}, 0, false);
    entity.AddComponent<Environment2DComponent>(ShaderFactory::Shader2D());
  }

  Camera2DController::~Camera2DController()
  {
    entity.Destroy();
  }

  void Camera2DController::OnEvent(Event& event)
  {
    Camera2DComponent& cam = entity.GetComponent<Camera2DComponent>();
    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      if(e.GetButton() == GREET_MOUSE_MIDDLE)
      {
        controller.pressPos = e.GetPosition();
        controller.camPressPos = cam.position;
        controller.isMoving = true;
      }
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_RELEASE))
    {
      MouseReleaseEvent& e = static_cast<MouseReleaseEvent&>(event);
      if(e.GetButton() == GREET_MOUSE_MIDDLE)
      {
        controller.isMoving = false;
      }
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_SCROLL))
    {
      MouseScrollEvent& e = static_cast<MouseScrollEvent&>(event);
      if(e.GetScrollVertical() > 0)
        cam.scale *= 0.9;
      else
        cam.scale *= 1.0f / 0.9f;
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      MouseMoveEvent& e = static_cast<MouseMoveEvent&>(event);
      if(controller.isMoving)
      {
        Mat3 viewProjInv = ~(cam.GetViewMatrix() * cam.GetProjectionMatrix());
        Vec2f pos = viewProjInv * e.GetPosition() - viewProjInv * controller.pressPos;
        cam.position = controller.camPressPos - pos;
      }
    }
    else if(EVENT_IS_TYPE(event, EventType::KEY_PRESS))
    {
      KeyPressEvent& e = static_cast<KeyPressEvent&>(event);
      if(e.GetButton() == GREET_KEY_F5)
      {
        cam.position = Vec2f{0, 0};
        cam.scale = Vec2f{1, 1};
        cam.rotation = 0;
      }
    }
  }
}
