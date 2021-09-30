#include "SpriteComponent.h"

#include <utils/MetaFileLoading.h>

namespace Greet
{
  SpriteComponent::SpriteComponent(const Ref<Texture2D>& texture, const Vec2f& texPos, const Vec2f& texSize)
    : texture{texture}, texPos{texPos}, texSize{texSize}
  {}

  SpriteComponent::SpriteComponent(const MetaFileClass& metaClass)
    : texture{MetaFileLoading::LoadTexture2D(metaClass)},
    texPos{MetaFileLoading::LoadTextureCoord(metaClass, texture, "texturePos", {0, 0})},
    texSize{MetaFileLoading::LoadTextureCoord(metaClass, texture, "textureSize", Vec2f{1, 1})}
  {}
}
