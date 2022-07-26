#include <scripting/NativeScript.h>

#include <ecs/components/Camera3DComponent.h>
#include <event/KeyEvent.h>
#include <event/MouseEvent.h>
#include <graphics/RenderCommand.h>
#include <input/Input.h>
#include <input/Input.h>
#include <math/Plane.h>

namespace Greet
{
  class Camera3DControllerScript : public NativeScript
  {
    struct Controller
    {
      bool mouseMiddleDown = false;
      bool mouseRightDown = false;
      bool keyShiftDown = false;
    };

    private:
      Controller controller;
      Vec3f pos;
      Vec3f rot;
      float distance;

      Vec3f vel;
      Vec3f rotVel;

      Mat4 pressInvPVMatrix;
      Vec3f pressPlanePos;
      Vec3f pressLinePos;
      Vec3f pressPos;
    public:
      Camera3DControllerScript(const Vec3f& pos = {}, const Vec3f& rot = {})
        : pos{pos}, rot{rot}, vel{}, rotVel{}, distance{5}
      {
      }

      void OnCreate() override
      {
        if(!entity.HasComponent<Camera3DComponent>())
        {
          Log::Warning("Camera3DController entity does not contain a Camera3DComponent");
          entity.AddComponent<Camera3DComponent>(Mat4::Identity(), 90.0f, 0.01f, 100.0f, true);
        };
      }

      void OnUpdate(float timeElapsed) override
      {
        Camera3DComponent& cam = entity.GetComponent<Camera3DComponent>();
        pos += Vec::Rotate(vel, {0, 1, 0}, -rot.y) * timeElapsed;
        rot += rotVel * timeElapsed;
        cam.SetViewMatrix(CalculateViewMatrix(pos, rot, distance));
      }

      void OnEvent(Event& event) override
      {
        float rotationSpeed = 4;
        float moveSpeed = 10;
        if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
        {
          MousePressEvent& e = static_cast<MousePressEvent&>(event);
          if(e.GetButton() == GREET_MOUSE_RIGHT)
          {
            Camera3DComponent& cam = entity.GetComponent<Camera3DComponent>();
            pressPlanePos = Plane{{0, 1, 0}, pos}.LineIntersection(cam.GetScreenToWorldCoordinate(e.GetPosition()));
            pressLinePos = Line{pos, {0, 1, 0}}.PointClosestFromLine(cam.GetScreenToWorldCoordinate(e.GetPosition()));
            pressPos = pos;
            pressInvPVMatrix = cam.GetInvPVMatrix();
          }
        }
        else if(EVENT_IS_TYPE(event, EventType::MOUSE_SCROLL))
        {
          MouseScrollEvent& e = static_cast<MouseScrollEvent&>(event);
          if(e.GetScrollVertical() > 0)
            distance *= 0.9;
          else
            distance /= 0.9;
        }
        else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
        {
          MouseMoveEvent& e = static_cast<MouseMoveEvent&>(event);
          if(Input::IsMouseDown(GREET_MOUSE_MIDDLE))
          {
            rot.y += 0.5 * e.GetDX() * RenderCommand::GetViewportAspect();
            rot.x -= 0.5 * e.GetDY();
          }
          if(Input::IsMouseDown(GREET_MOUSE_RIGHT))
          {
            Camera3DComponent& cam = entity.GetComponent<Camera3DComponent>();

            Line line;
            line.pos = pressInvPVMatrix * Vec3f(e.GetX(), e.GetY(), -1.0);
            Vec3f far = pressInvPVMatrix * Vec3f(e.GetX(), e.GetY(), 1.0);
            line.dir = far - line.pos;

            if(Input::IsKeyDown(GREET_KEY_LEFT_SHIFT))
            {
              pos = pressPos + Line{pressPos, {0, 1, 0}}.PointClosestFromLine(line) - pressLinePos;
            }
            else
            {
              Line line;
              line.pos = pressInvPVMatrix * Vec3f(e.GetX(), e.GetY(), -1.0);
              Vec3f far = pressInvPVMatrix * Vec3f(e.GetX(), e.GetY(), 1.0);
              line.dir = far - line.pos;

              pos = pressPos + Plane{{0, 1, 0}, pressPos}.LineIntersection(line) - pressPlanePos;
            }
          }
        }
      }

      Mat4 CalculateViewMatrix(const Vec3f& pos, const Vec3f& rot, float distance)
      {
        return Mat4::Translate(0, 0, -distance) * Mat4::RotateX(rot.x) * Mat4::RotateY(rot.y) * Mat4::Translate(pos);
      }
  };
}
