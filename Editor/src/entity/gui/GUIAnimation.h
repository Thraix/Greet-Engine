#pragma once

#include <common/Memory.h>
#include <ecs/components/AnimationComponent.h>

namespace Greet
{
  class Component;
  class Container;
  class EntityManager;

  class GUIAnimation
  {
    DELETE_COPY_AND_MOVE(GUIAnimation);

    EntityManager* entityManager;
    Component* guiAnimation;

    public:
      GUIAnimation(EntityManager* entityManager, Container* parent);
      virtual ~GUIAnimation();

      void Update(const AnimationComponent& animationComponent);
      void AttachTo(Container* container, AnimationComponent& animationComponent);

    private:
      void GUITextBoxSprites(Component* component,
                             const std::string& before,
                             const std::string& after);
      void GUITextBoxKeytime(Component* component,
                             const std::string& before,
                             const std::string& after);
  };
}
