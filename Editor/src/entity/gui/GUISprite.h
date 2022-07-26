#pragma once

#include <ecs/components/SpriteComponent.h>

namespace Greet
{
  class EntityManager;
  class Component;
  class Container;

  class GUISprite
  {
    DELETE_COPY_AND_MOVE(GUISprite);

    EntityManager* entityManager;
    Component* guiSprite;

    public:
      GUISprite(EntityManager* entityManager, Container* parent);
      virtual ~GUISprite();

      void Update(const SpriteComponent& spriteComponent);
      void AttachTo(Container* container, SpriteComponent& spriteComponent);

    private:
      void GUITextBoxTexturePath(Component* component,
                                 const std::string& before,
                                 const std::string& after);
      void GUITextBoxSpritePosX(Component* component,
                                const std::string& before,
                                const std::string& after);
      void GUITextBoxSpritePosY(Component* component,
                                const std::string& before,
                                const std::string& after);
      void GUITextBoxSpriteSizeX(Component* component,
                                 const std::string& before,
                                 const std::string& after);
      void GUITextBoxSpriteSizeY(Component* component,
                                 const std::string& before,
                                 const std::string& after);
  };
}
