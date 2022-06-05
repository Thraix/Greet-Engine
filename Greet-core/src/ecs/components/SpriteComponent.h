#pragma once

#include <graphics/textures/Texture2D.h>
#include <math/Vec2.h>
#include <utils/MetaFile.h>

namespace Greet
{
  struct SpriteComponent
  {
    Ref<Texture2D> texture;
    int spriteSheetX;
    int spriteSheetY;
    int spriteSheetWidth;  // Amount of sprites in the x-axis
    int spriteSheetHeight; // Amount of sprites in the y-axis

    SpriteComponent(const Ref<Texture2D>& texture, int spriteSheetX = 0, int spriteSheetY = 0, int spriteSheetWidth = 1, int spriteSheetHeight = 1);
    SpriteComponent(const MetaFileClass& metaClass);

    Vec2f GetTexPos() const;
    Vec2f GetTexSize() const;
  };
}
