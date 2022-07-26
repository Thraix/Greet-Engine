#include "Camera3DController.h"

#include <ecs/components/Camera3DComponent.h>
#include <ecs/components/UUIDComponent.h>
#include <ecs/components/TagComponent.h>
#include <ecs/components/Environment3DComponent.h>
#include <graphics/shaders/ShaderFactory.h>
#include <graphics/textures/TextureManager.h>
#include <event/KeyEvent.h>
#include <event/MouseEvent.h>
#include <graphics/RenderCommand.h>
#include <input/Input.h>
#include <math/Plane.h>

namespace Greet
{
  Camera3DController::Camera3DController(ECSManager* ecs, const Vec3f& pos, const Vec3f& rot)
    : pos{pos}, rot{rot}, vel{}, rotVel{}, distance{5}
  {
    entity = Entity{ecs, ecs->CreateEntity()};
    entity.AddComponent<UUIDComponent>(UUID{});
    entity.AddComponent<TagComponent>("EditorCamera3D");
    entity.AddComponent<Camera3DComponent>(Mat4::Identity(), 90, 0.001, 1000.0f, false);
    entity.AddComponent<Environment3DComponent>(ShaderFactory::ShaderSkybox(), TextureManager::LoadCubeMap("res/textures/skybox.meta"));
  }

  void Camera3DController::OnUpdate(float timeElapsed)
  {
    Camera3DComponent& cam = entity.GetComponent<Camera3DComponent>();
    pos += Vec::Rotate(vel, {0, 1, 0}, -rot.y) * timeElapsed;
    rot += rotVel * timeElapsed;
    cam.SetViewMatrix(CalculateViewMatrix(pos, rot, distance));
  }

  void Camera3DController::OnEvent(Event& event)
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

  Mat4 Camera3DController::CalculateViewMatrix(const Vec3f& pos, const Vec3f& rot, float distance)
  {
    return Mat4::Translate(0, 0, -distance) * Mat4::RotateX(rot.x) * Mat4::RotateY(rot.y) * Mat4::Translate(pos);
  }
}
