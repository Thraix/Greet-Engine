#include "SpriteComponent.h"

#include <utils/MetaFileLoading.h>
#include <graphics/textures/TextureManager.h>

namespace Greet
{
  SpriteComponent::SpriteComponent(const std::string& texture, const Vec2i& spriteSheetPos, const Vec2i& spriteSheetSize)
    : texture{TextureManager::LoadTexture2D(texture)},
    texturePath{texture},
    spriteSheetPos{spriteSheetPos},
    spriteSheetSize{spriteSheetSize}
  {}

  SpriteComponent::SpriteComponent(const MetaFileClass& metaClass)
    : texture{MetaFileLoading::LoadTexture2D(metaClass)},
    texturePath{MetaFileLoading::LoadString(metaClass, "texture", "")},
    spriteSheetPos{MetaFileLoading::LoadVec2i(metaClass, "spritesheet-pos", Vec2i{0, 0})},
    spriteSheetSize{MetaFileLoading::LoadVec2i(metaClass, "spritesheet-size", Vec2i{1, 1})}
  {}

  Vec2f SpriteComponent::GetTexPos() const
  {
    // TODO(optimize): Avoid division (store the vector somewhere?)
    return {spriteSheetPos.x / (float)spriteSheetSize.x, spriteSheetPos.y / (float)spriteSheetSize.y};
  }

  Vec2f SpriteComponent::GetTexSize() const
  {
    // TODO(optimize): Avoid division (store the vector somewhere?)
    return Vec2f{1 / (float)spriteSheetSize.x, 1 / (float)spriteSheetSize.y};
  }

  MetaFile& operator<<(MetaFile& metaFile, const SpriteComponent& component)
  {
    MetaFileClass meta;
    meta.AddValue("texture", component.texturePath);
    meta.AddValue("spritesheet-pos", std::to_string(component.spriteSheetPos.x) + " " + std::to_string(component.spriteSheetPos.y));
    meta.AddValue("spritesheet-size", std::to_string(component.spriteSheetSize.x) + " " + std::to_string(component.spriteSheetSize.y));
    metaFile.AddMetaClass("SpriteComponent", meta);
    return metaFile;
  }
}
