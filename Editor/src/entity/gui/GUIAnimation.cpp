#include "GUIAnimation.h"

#include "GUIHelper.h"
#include "../EntityManager.h"

#include <ecs/components/AnimationComponent.h>
#include <graphics/gui/Component.h>
#include <graphics/gui/Container.h>
#include <graphics/gui/TextBox.h>

namespace Greet
{
  GUIAnimation::GUIAnimation(EntityManager* entityManager, Container* parent)
    : entityManager{entityManager}, guiAnimation{ComponentFactory::GetComponent("res/guis/AnimationComponent.xml", parent)}
  {
    guiAnimation->GetComponentByName<TextBox>("sprites")
      ->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxSprites));

    guiAnimation->GetComponentByName<TextBox>("keytime")
      ->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxKeytime));
  }

  GUIAnimation::~GUIAnimation()
  {
    delete guiAnimation;
  }

  void GUIAnimation::Update(const AnimationComponent& animationComponent)
  {
    guiAnimation->GetComponentByName<TextBox>("sprites")->SetText(GUIHelper::IntToString(animationComponent.animationCount));
    guiAnimation->GetComponentByName<TextBox>("keytime")->SetText(GUIHelper::FloatToString(animationComponent.keytime));
  }

  void GUIAnimation::AttachTo(Container* container, AnimationComponent& animationComponent)
  {
    container->AddComponent(guiAnimation);
    Update(animationComponent);
  }

  void GUIAnimation::GUITextBoxSprites(Component* component,
                                       const std::string& before,
                                       const std::string& after)
  {
    if(!entityManager->GetSelectedEntity().HasComponent<AnimationComponent>())
      return;

    entityManager->GetSelectedEntity().GetComponent<AnimationComponent>().animationCount = GUIHelper::StringToInt(after);
  }

  void GUIAnimation::GUITextBoxKeytime(Component* component,
                                       const std::string& before,
                                       const std::string& after)
  {
    if(!entityManager->GetSelectedEntity().HasComponent<AnimationComponent>())
      return;
    entityManager->GetSelectedEntity().GetComponent<AnimationComponent>().keytime = GUIHelper::StringToFloat(after);
  }
}
