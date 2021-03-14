#include <scripting/NativeScript.h>
#include <ecs/ECSComponents.h>
#include <input/Input.h>
#include <event/KeyEvent.h>

class CameraControllerScript : public Greet::NativeScript
{
  struct Controller
  {
    bool forwardKey = false;
    bool backKey = false;
    bool leftKey = false;
    bool rightKey = false;
    bool upKey = false;
    bool downKey = false;

    bool yawPlusKey = false;
    bool yawMinusKey = false;
    bool pitchPlusKey = false;
    bool pitchMinusKey = false;
  };

  private:
    Controller controller;
    Greet::Vec3f pos;
    Greet::Vec3f rot;

    Greet::Vec3f vel;
    Greet::Vec3f rotVel;
  public:
    CameraControllerScript(const Greet::Vec3f& pos = {}, const Greet::Vec3f& rot = {})
      : pos{pos}, rot{rot}, vel{}, rotVel{}
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
      cam.SetViewMatrix(Greet::Mat4::ViewMatrix(pos, rot));
    }

    void OnEvent(Greet::Event& event) override
    {
      float rotationSpeed = 4;
      float moveSpeed = 10;
      if(EVENT_IS_TYPE(event, Greet::EventType::KEY_PRESS))
      {
        Greet::KeyPressEvent& e = static_cast<Greet::KeyPressEvent&>(event);
        if(e.IsRepeat())
          return;
        switch(e.GetButton())
        {
          case GREET_KEY_DOWN:
            controller.pitchMinusKey = true; break;
          case GREET_KEY_UP:
            controller.pitchPlusKey = true; break;
          case GREET_KEY_LEFT:
            controller.yawMinusKey= true; break;
          case GREET_KEY_RIGHT:
            controller.yawPlusKey = true; break;
          case GREET_KEY_W:
            controller.forwardKey = true; break;
          case GREET_KEY_S:
            controller.backKey = true; break;
          case GREET_KEY_A:
            controller.leftKey = true; break;
          case GREET_KEY_D:
            controller.rightKey = true; break;
          case GREET_KEY_LEFT_SHIFT:
            controller.downKey = true; break;
          case GREET_KEY_SPACE:
            controller.upKey = true; break;
        }
      }
      else if(EVENT_IS_TYPE(event, Greet::EventType::KEY_RELEASE))
      {
        Greet::KeyReleaseEvent& e = static_cast<Greet::KeyReleaseEvent&>(event);
        switch(e.GetButton())
        {
          case GREET_KEY_DOWN:
            controller.pitchMinusKey = false; break;
          case GREET_KEY_UP:
            controller.pitchPlusKey = false; break;
          case GREET_KEY_LEFT:
            controller.yawMinusKey= false; break;
          case GREET_KEY_RIGHT:
            controller.yawPlusKey = false; break;
          case GREET_KEY_W:
            controller.forwardKey = false; break;
          case GREET_KEY_S:
            controller.backKey = false; break;
          case GREET_KEY_A:
            controller.leftKey = false; break;
          case GREET_KEY_D:
            controller.rightKey = false; break;
          case GREET_KEY_LEFT_SHIFT:
            controller.downKey = false; break;
          case GREET_KEY_SPACE:
            controller.upKey = false; break;
        }
      }
      rotVel = {};
      vel = {};
      if(controller.pitchMinusKey) rotVel.x -= rotationSpeed;
      if(controller.pitchPlusKey) rotVel.x += rotationSpeed;
      if(controller.yawMinusKey) rotVel.y += rotationSpeed;
      if(controller.yawPlusKey) rotVel.y -= rotationSpeed;

      if(controller.forwardKey) vel.z -= moveSpeed;
      if(controller.backKey) vel.z += moveSpeed;
      if(controller.leftKey) vel.x -= moveSpeed;
      if(controller.rightKey) vel.x += moveSpeed;
      if(controller.upKey) vel.y += moveSpeed;
      if(controller.downKey) vel.y -= moveSpeed;
    }
};
