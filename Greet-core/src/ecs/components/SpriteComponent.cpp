#include "SpriteComponent.h"

#include <utils/MetaFileLoading.h>

namespace Greet
{
  SpriteComponent::SpriteComponent(const Ref<Texture2D>& texture, int spriteSheetX, int spriteSheetY, int spriteSheetWidth, int spriteSheetHeight)
    : texture{texture},
    spriteSheetX{spriteSheetX},
    spriteSheetY{spriteSheetY},
    spriteSheetWidth{spriteSheetWidth},
    spriteSheetHeight{spriteSheetHeight}
  {}

  SpriteComponent::SpriteComponent(const MetaFileClass& metaClass)
    : texture{MetaFileLoading::LoadTexture2D(metaClass)},
    spriteSheetX{MetaFileLoading::LoadInt(metaClass, "spritesheet-x", 0)},
    spriteSheetY{MetaFileLoading::LoadInt(metaClass, "spritesheet-y", 0)},
    spriteSheetWidth{MetaFileLoading::LoadInt(metaClass, "spritesheet-width", 1)},
    spriteSheetHeight{MetaFileLoading::LoadInt(metaClass, "spritesheet-height", 1)}
  {}

  Vec2f SpriteComponent::GetTexPos() const
  {
    // TODO(optimize): Avoid division (store the vector somewhere?)
    return {spriteSheetX / (float)spriteSheetWidth, spriteSheetY / (float)spriteSheetHeight};
  }

  Vec2f SpriteComponent::GetTexSize() const
  {
    // TODO(optimize): Avoid division (store the vector somewhere?)
    return Vec2f{1 / (float)spriteSheetWidth, 1 / (float)spriteSheetHeight};
  }
}
