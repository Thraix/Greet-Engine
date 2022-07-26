#include "GUISprite.h"

#include "../EntityManager.h"
#include "GUIHelper.h"

#include <graphics/gui/Component.h>
#include <graphics/gui/Container.h>
#include <graphics/gui/TextBox.h>

namespace Greet
{
  GUISprite::GUISprite(EntityManager* entityManager, Container* parent)
    : entityManager{entityManager}, guiSprite{ComponentFactory::GetComponent("res/guis/SpriteComponent.xml", parent)}
  {
    guiSprite->GetComponentByName<TextBox>("texturePath")
      ->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxTexturePath));

    guiSprite->GetComponentByName<TextBox>("spritePosX")
      ->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxSpritePosX));
    guiSprite->GetComponentByName<TextBox>("spritePosY")
      ->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxSpritePosY));

    guiSprite->GetComponentByName<TextBox>("spriteSizeX")
      ->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxSpriteSizeX));
    guiSprite->GetComponentByName<TextBox>("spriteSizeY")
      ->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxSpriteSizeY));
  }

  GUISprite::~GUISprite()
  {
    delete guiSprite;
  }

  void GUISprite::Update(const SpriteComponent& spriteComponent)
  {
    guiSprite->GetComponentByName<TextBox>("texturePath")->SetText(spriteComponent.texturePath);

    guiSprite->GetComponentByName<TextBox>("spritePosX")->SetText(GUIHelper::IntToString(spriteComponent.spriteSheetPos.x));
    guiSprite->GetComponentByName<TextBox>("spritePosY")->SetText(GUIHelper::IntToString(spriteComponent.spriteSheetPos.y));

    guiSprite->GetComponentByName<TextBox>("spriteSizeX")->SetText(GUIHelper::IntToString(spriteComponent.spriteSheetSize.x));
    guiSprite->GetComponentByName<TextBox>("spriteSizeY")->SetText(GUIHelper::IntToString(spriteComponent.spriteSheetSize.y));
  }

  void GUISprite::AttachTo(Container* container, SpriteComponent& spriteComponent)
  {
    container->AddComponent(guiSprite);
    Update(spriteComponent);
  }


  void GUISprite::GUITextBoxTexturePath(Component* component,
                                        const std::string& before,
                                        const std::string& after)
  {
    if(FileUtils::FileExist(after))
    {
      if(!entityManager->GetSelectedEntity().HasComponent<SpriteComponent>())
        return;

      entityManager->GetSelectedEntity().GetComponent<SpriteComponent>().SetTexture(after);
    }
  }

  void GUISprite::GUITextBoxSpritePosX(Component* component,
                                       const std::string& before,
                                       const std::string& after)
  {
    if(!entityManager->GetSelectedEntity().HasComponent<SpriteComponent>())
      return;

    entityManager->GetSelectedEntity().GetComponent<SpriteComponent>().spriteSheetPos.x = GUIHelper::StringToInt(after);
  }

  void GUISprite::GUITextBoxSpritePosY(Component* component,
                                       const std::string& before,
                                       const std::string& after)
  {
    if(!entityManager->GetSelectedEntity().HasComponent<SpriteComponent>())
      return;
    entityManager->GetSelectedEntity().GetComponent<SpriteComponent>().spriteSheetPos.y = GUIHelper::StringToInt(after);
  }

  void GUISprite::GUITextBoxSpriteSizeX(Component* component,
                                        const std::string& before,
                                        const std::string& after)
  {
    if(!entityManager->GetSelectedEntity().HasComponent<SpriteComponent>())
      return;

    entityManager->GetSelectedEntity().GetComponent<SpriteComponent>().spriteSheetSize.x = GUIHelper::StringToInt(after);
  }

  void GUISprite::GUITextBoxSpriteSizeY(Component* component,
                                        const std::string& before,
                                        const std::string& after)
  {
    if(!entityManager->GetSelectedEntity().HasComponent<SpriteComponent>())
      return;

    entityManager->GetSelectedEntity().GetComponent<SpriteComponent>().spriteSheetSize.y = GUIHelper::StringToInt(after);
  }
}
