#include <scripting/NativeScript.h>

#include <ecs/components/Camera2DComponent.h>
#include <event/KeyEvent.h>
#include <event/MouseEvent.h>
#include <graphics/RenderCommand.h>
#include <input/Input.h>
#include <input/Input.h>
#include <math/Plane.h>

namespace Greet
{
  class Camera2DControllerScript : public NativeScript
  {
    struct Controller
    {
      Vec2f pressPos;
      Vec2f camPressPos;
      bool mouseMiddleDown = false;
      bool mouseRightDown = false;
      bool keyShiftDown = false;
    };

    private:
      Controller controller;

    public:
      Camera2DControllerScript() {}

      void OnCreate() override
      {
        if(!entity.HasComponent<Camera2DComponent>())
        {
          Log::Warning("Camera2DController entity does not contain a Camera2DComponent");
          entity.AddComponent<Camera2DComponent>(Vec2f{0, 0}, Vec2f{1, 1}, 0.0f, true);
        };
      }

      void OnUpdate(float timeElapsed) override
      {}

      void OnEvent(Event& event) override
      {
        Camera2DComponent& cam = entity.GetComponent<Camera2DComponent>();
        if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
        {
          MousePressEvent& e = static_cast<MousePressEvent&>(event);
          if(e.GetButton() == GREET_MOUSE_MIDDLE)
          {
            controller.pressPos = e.GetPosition();
            controller.camPressPos = cam.position;
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
          if(Input::IsMouseDown(GREET_MOUSE_MIDDLE))
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
  };
}
