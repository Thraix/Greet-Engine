#include <scripting/NativeScript.h>

#include <ecs/components/MeshComponent.h>
#include <graphics/models/MeshFactory.h>
#include <utils/Noise.h>

class TerrainGeneratorScript : public Greet::NativeScript
{
  public:
    void OnCreate() override
    {
      ASSERT(entity.HasComponent<Greet::MeshComponent>(), "Terrain does not contain meshcomponent");
      Greet::MeshComponent& mesh = entity.GetComponent<Greet::MeshComponent>();
      Greet::Pointer<float> noise = Greet::Noise::GenNoise(100, 100, 5, 8, 8, 0.5f);
      ProcessNoise(noise);
      Greet::MeshData data{Greet::MeshFactory::Grid({99, 99}, noise, {0.0f, 0.0f, 0.0f}, {99.0f, 1.0f, 99.0f})};
      mesh.mesh.reset(new Greet::Mesh{data});
    }

    void ProcessNoise(Greet::Pointer<float>& noise)
    {
      for(float& f : noise)
      {
        if (f > 0.58 && f < 0.65)
        {
          f = (pow(f - 0.58, 0.6) + 0.58);
        }
        else if(f > 0.65)
        {
          f = (pow(f - 0.58, 0.6) + 0.58);
        }
      }
    }
};

REGISTER_NATIVE_SCRIPT(TerrainGeneratorScript);
