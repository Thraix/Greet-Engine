#pragma once

#include <graphics/textures/Texture2D.h>
#include <math/Vec2.h>
#include <utils/MetaFile.h>

namespace Greet
{
  struct SpriteComponent
  {
    Ref<Texture2D> texture;
    Vec2f texPos;
    Vec2f texSize;

    SpriteComponent(const Ref<Texture2D>& texture, const Vec2f& texPos, const Vec2f& texSize);
    SpriteComponent(const MetaFileClass& metaClass);
  };
}
