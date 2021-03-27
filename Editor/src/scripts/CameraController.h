#include <scripting/NativeScript.h>
#include <ecs/ECSComponents.h>
#include <input/Input.h>
#include <event/KeyEvent.h>
#include <event/MouseEvent.h>
#include <input/Input.h>

class CameraControllerScript : public Greet::NativeScript
{
  struct Controller
  {
    bool mouseMiddleDown = false;
    bool mouseRightDown = false;
    bool keyShiftDown = false;
  };

  private:
    Controller controller;
    Greet::Vec3f pos;
    Greet::Vec3f rot;
    float distance;

    Greet::Vec3f vel;
    Greet::Vec3f rotVel;

    Greet::Mat4 pressInvPVMatrix;
    Greet::Vec3f pressPlanePos;
    Greet::Vec3f pressLinePos;
    Greet::Vec3f pressPos;
  public:
    CameraControllerScript(const Greet::Vec3f& pos = {}, const Greet::Vec3f& rot = {})
      : pos{pos}, rot{rot}, vel{}, rotVel{}, distance{5}
    {
    }

    void OnCreate() override
    {
      if(!entity.HasComponent<Greet::Camera3DComponent>())
      {
        Greet::Log::Warning("CameraController entity does not contain a Camera3DComponent");
        entity.AddComponent<Greet::Camera3DComponent>(Greet::Mat4::Identity(), 90.0f, 0.01f, 100.0f, true);
      };
    }

    void OnUpdate(float timeElapsed) override
    {
      Greet::Camera3DComponent& cam = entity.GetComponent<Greet::Camera3DComponent>();
      pos += Greet::Vec::Rotate(vel, {0, 1, 0}, -rot.y) * timeElapsed;
      rot += rotVel * timeElapsed;
      cam.SetViewMatrix(CalculateViewMatrix(pos, rot, distance));
    }

    void OnEvent(Greet::Event& event) override
    {
      float rotationSpeed = 4;
      float moveSpeed = 10;
      if(EVENT_IS_TYPE(event, Greet::EventType::MOUSE_PRESS))
      {
        Greet::MousePressEvent& e = static_cast<Greet::MousePressEvent&>(event);
        if(e.GetButton() == GREET_MOUSE_RIGHT)
        {
          Greet::Camera3DComponent& cam = entity.GetComponent<Greet::Camera3DComponent>();
          pressPlanePos = Greet::Plane{{0, 1, 0}, pos}.LineIntersection(cam.GetScreenToWorldCoordinate(e.GetPosition()));
          pressLinePos = Greet::Line{pos, {0, 1, 0}}.PointClosestFromLine(cam.GetScreenToWorldCoordinate(e.GetPosition()));
          pressPos = pos;
          pressInvPVMatrix = cam.GetInvPVMatrix();
        }
      }
      else if(EVENT_IS_TYPE(event, Greet::EventType::MOUSE_SCROLL))
      {
        Greet::MouseScrollEvent& e = static_cast<Greet::MouseScrollEvent&>(event);
        if(e.GetScrollVertical() > 0)
          distance *= 0.9;
        else
          distance /= 0.9;
      }
      else if(EVENT_IS_TYPE(event, Greet::EventType::MOUSE_MOVE))
      {
        Greet::MouseMoveEvent& e = static_cast<Greet::MouseMoveEvent&>(event);
        if(Greet::Input::IsMouseDown(GREET_MOUSE_MIDDLE))
        {
          rot.y += 0.5 * e.GetDX() * Greet::RenderCommand::GetViewportAspect();
          rot.x -= 0.5 * e.GetDY();
        }
        if(Greet::Input::IsMouseDown(GREET_MOUSE_RIGHT))
        {
          Greet::Camera3DComponent& cam = entity.GetComponent<Greet::Camera3DComponent>();

          Greet::Line line;
          line.pos = pressInvPVMatrix * Greet::Vec3f(e.GetX(), e.GetY(), -1.0);
          Greet::Vec3f far = pressInvPVMatrix * Greet::Vec3f(e.GetX(), e.GetY(), 1.0);
          line.dir = far - line.pos;

          if(Greet::Input::IsKeyDown(GREET_KEY_LEFT_SHIFT))
          {
            pos = pressPos + Greet::Line{pressPos, {0, 1, 0}}.PointClosestFromLine(line) - pressLinePos;
          }
          else
          {
            Greet::Line line;
            line.pos = pressInvPVMatrix * Greet::Vec3f(e.GetX(), e.GetY(), -1.0);
            Greet::Vec3f far = pressInvPVMatrix * Greet::Vec3f(e.GetX(), e.GetY(), 1.0);
            line.dir = far - line.pos;

            pos = pressPos + Greet::Plane{{0, 1, 0}, pressPos}.LineIntersection(line) - pressPlanePos;
          }
        }
      }
    }

    Greet::Mat4 CalculateViewMatrix(const Greet::Vec3f& pos, const Greet::Vec3f& rot, float distance)
    {
      return Greet::Mat4::Translate(0, 0, -distance) * Greet::Mat4::RotateX(rot.x) * Greet::Mat4::RotateY(rot.y) * Greet::Mat4::Translate(pos);
    }
};
