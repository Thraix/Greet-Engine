#pragma once

#include <graphics/textures/Texture2D.h>
#include <math/Vec2.h>
#include <utils/MetaFile.h>

namespace Greet
{
  struct SpriteComponent
  {
    Ref<Texture2D> texture;
    std::string texturePath; // Stored to be able to be serialized, might look for a better solution at some point
                             // (maybe the path should be stored directly in the texture?)
    Vec2i spriteSheetPos;
    Vec2i spriteSheetSize;

    SpriteComponent(const std::string& texture = "", const Vec2i& spriteSheetPos = Vec2i{0, 0}, const Vec2i& spriteSheetSize = Vec2i{1, 1});
    SpriteComponent(const MetaFileClass& metaClass);

    Vec2f GetTexPos() const;
    Vec2f GetTexSize() const;

    void SetTexture(const std::string& texturePath);

    friend MetaFile& operator<<(MetaFile& metaFile, const SpriteComponent& component);
  };
}
