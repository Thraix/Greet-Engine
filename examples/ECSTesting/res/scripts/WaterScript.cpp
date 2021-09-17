#include <scripting/NativeScript.h>
#include <ecs/components/MaterialComponent.h>
#include <graphics/models/MeshFactory.h>
#include <utils/Noise.h>


struct WaterScript : public Greet::NativeScript
{
  float time = 0.0f;

  void OnCreate() override
  {
    ASSERT(entity.HasComponent<Greet::MaterialComponent>(), "Water does not contain MaterialComponent");
    Greet::MaterialComponent& material = entity.GetComponent<Greet::MaterialComponent>();
    material.material.GetShader()->Enable();
    material.material.SetSpecularExponent(50);
    material.material.SetSpecularStrength(0.4);
    material.material.GetShader()->Disable();
  }

  void OnUpdate(float timeElapsed) override
  {
    time += timeElapsed;
    ASSERT(entity.HasComponent<Greet::MaterialComponent>(), "Water does not contain MaterialComponent");
    Greet::MaterialComponent& material = entity.GetComponent<Greet::MaterialComponent>();
    material.material.GetShader()->Enable();
    material.material.GetShader()->SetUniform1f("uTime", time);
    material.material.GetShader()->Disable();
  }
};

REGISTER_NATIVE_SCRIPT(WaterScript);
