#include "ECSTesting.h"
#include "ecs/components/Environment3DComponent.h"
#include "ecs/components/MaterialComponent.h"

using namespace Greet;

struct PrintComponent
{
};

class ECSSceneExt : public ECSScene
{
  public:
    ECSSceneExt(const std::string& scenePath)
      : ECSScene{scenePath}
    {}

    virtual ~ECSSceneExt()
    {}

    void LoadExtComponents(Entity& entity, const MetaFile& metaClass) override
    {}

    void UpdateAfter(float timeElapsed) override
    {}
};

ECSTesting::ECSTesting()
  : App{"ECSTesting", 1440, 1440 * 9 / 16}
{
  SetFrameCap(60);
}

ECSTesting::~ECSTesting()
{
}

void ECSTesting::Init()
{
  FontManager::Add("noto", FontContainer("res/fonts/noto.ttf"));
  Ref<GUIScene> gui = NewRef<GUIScene>();
  Frame* frame = FrameFactory::GetFrame("res/guis/gui.xml");
  gui->AddFrameQueued(frame);
  GlobalSceneManager::GetSceneManager().Add2DScene(gui, "gui");
  sceneView = frame->GetComponentByName<SceneView>("sceneView");
  ASSERT(sceneView, "SceneView did not exist in gui");
  Ref<ECSSceneExt> scene = NewRef<ECSSceneExt>("res/scenes/scene.meta");
  sceneView->GetSceneManager().Add3DScene(scene, "ecs");
  gui->RequestFocusQueued(sceneView);


  Entity glowCube{scene->GetManager()};
  Entity environment3D{scene->GetManager()};
  scene->GetManager()->Each<Greet::TagComponent, Greet::MaterialComponent>(
    [&](EntityID entity, Greet::TagComponent& tagComponent, Greet::MaterialComponent& materialComponent)
    {
      if(tagComponent.tag == "GlowCube")
      {
        glowCube = entity;
      }
    });
  scene->GetManager()->Each<Greet::Environment3DComponent>(
    [&](EntityID entity, Greet::Environment3DComponent& environment3DComponent)
    {
      environment3D = entity;
    });
  ASSERT(glowCube, "Could not find glowcube entity");
  ASSERT(environment3D, "Could not find environment3D entity");

  frame->GetComponentByName<Slider>("ambient_slider")->SetOnValueChangeCallback(
    [glowCube, frame](Slider* slider, float oldValue, float newValue)
    {
      MaterialComponent& mat = glowCube.GetComponent<Greet::MaterialComponent>();
      mat.material.SetAmbient(newValue);
      frame->GetComponentByName<Label>("ambient_label")->SetText(std::to_string(newValue));
    });

  frame->GetComponentByName<Slider>("intensity_slider")->SetOnValueChangeCallback(
    [glowCube, frame](Slider* slider, float oldValue, float newValue)
    {
      MaterialComponent& mat = glowCube.GetComponent<Greet::MaterialComponent>();
      mat.material.SetIntensity(newValue);
      frame->GetComponentByName<Label>("intensity_label")->SetText(std::to_string(newValue));
    });

  frame->GetComponentByName<Slider>("bloom_threshold_slider")->SetOnValueChangeCallback(
    [environment3D, frame](Slider* slider, float oldValue, float newValue)
    {
      Environment3DComponent& env = environment3D.GetComponent<Environment3DComponent>();
      env.bloomThreshold = newValue;
      frame->GetComponentByName<Label>("bloom_threshold_label")->SetText(std::to_string(newValue));
    });

  frame->GetComponentByName<Slider>("bloom_falloff_slider")->SetOnValueChangeCallback(
    [environment3D, frame](Slider* slider, float oldValue, float newValue)
    {
      Environment3DComponent& env = environment3D.GetComponent<Environment3DComponent>();
      env.bloomFalloff = newValue;
      frame->GetComponentByName<Label>("bloom_falloff_label")->SetText(std::to_string(newValue));
    }
  );
}

void ECSTesting::Destruct()
{
  sceneView->GetSceneManager().Remove3DScene("ecs");
  GlobalSceneManager::GetSceneManager().Remove2DScene("gui");
}

void ECSTesting::Tick()
{
}

void ECSTesting::Update(float timeElapsed)
{
}

void ECSTesting::Render()
{
}

void ECSTesting::OnEvent(Greet::Event& event)
{
}
