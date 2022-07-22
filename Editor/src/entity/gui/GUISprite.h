#pragma once

#include <graphics/gui/Container.h>
#include <ecs/components/SpriteComponent.h>
#include <functional>

class EntityManager;

class GUISprite
{
  EntityManager* entityManager;

  Greet::Component* guiSprite;

  DELETE_COPY_AND_MOVE(GUISprite);

  public:
    GUISprite(EntityManager* entityManager, Greet::Container* parent);
    virtual ~GUISprite();

    void Update(const Greet::SpriteComponent& spriteComponent);
    void AttachTo(Greet::Container* container, Greet::SpriteComponent& spriteComponent);

  private:
    void GUITextBoxTexturePath(Greet::Component* component,
                               const std::string& before,
                               const std::string& after);
    void GUITextBoxSpritePosX(Greet::Component* component,
                              const std::string& before,
                              const std::string& after);
    void GUITextBoxSpritePosY(Greet::Component* component,
                              const std::string& before,
                              const std::string& after);
    void GUITextBoxSpriteSizeX(Greet::Component* component,
                               const std::string& before,
                               const std::string& after);
    void GUITextBoxSpriteSizeY(Greet::Component* component,
                               const std::string& before,
                               const std::string& after);
};
