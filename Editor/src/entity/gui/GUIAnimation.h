#pragma once

#include "ecs/components/AnimationComponent.h"
#include <graphics/gui/Container.h>
#include <ecs/components/SpriteComponent.h>
#include <functional>

class EntityManager;

class GUIAnimation
{
  EntityManager* entityManager;

  Greet::Component* guiAnimation;

  DELETE_COPY_AND_MOVE(GUIAnimation);

  public:
    GUIAnimation(EntityManager* entityManager, Greet::Container* parent);
    virtual ~GUIAnimation();

    void Update(const Greet::AnimationComponent& animationComponent);
    void AttachTo(Greet::Container* container, Greet::AnimationComponent& animationComponent);

  private:
    void GUITextBoxSprites(Greet::Component* component,
                           const std::string& before,
                           const std::string& after);
    void GUITextBoxKeytime(Greet::Component* component,
                           const std::string& before,
                           const std::string& after);
};
