#include "GUIAnimation.h"

#include "GUIHelper.h"
#include "../EntityManager.h"
#include "ecs/components/AnimationComponent.h"
#include <graphics/gui/TextBox.h>

using namespace Greet;

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

void GUIAnimation::Update(const Greet::AnimationComponent& animationComponent)
{
  guiAnimation->GetComponentByName<TextBox>("sprites")->SetText(GUIHelper::IntToString(animationComponent.animationCount));
  guiAnimation->GetComponentByName<TextBox>("keytime")->SetText(GUIHelper::FloatToString(animationComponent.keytime));
}

void GUIAnimation::AttachTo(Greet::Container* container, Greet::AnimationComponent& animationComponent)
{
  container->AddComponent(guiAnimation);
  Update(animationComponent);
}

void GUIAnimation::GUITextBoxSprites(Greet::Component* component,
                                     const std::string& before,
                                     const std::string& after)
{
  if(!entityManager->GetSelectedEntity().HasComponent<SpriteComponent>())
    return;

  entityManager->GetSelectedEntity().GetComponent<AnimationComponent>().animationCount = GUIHelper::StringToInt(after);
}

void GUIAnimation::GUITextBoxKeytime(Greet::Component* component,
                                     const std::string& before,
                                     const std::string& after)
{
  if(!entityManager->GetSelectedEntity().HasComponent<SpriteComponent>())
    return;
  entityManager->GetSelectedEntity().GetComponent<AnimationComponent>().keytime = GUIHelper::StringToFloat(after);
}

