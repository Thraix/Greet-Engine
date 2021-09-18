#pragma once

#include <graphics/textures/Texture2D.h>
#include <math/Vec2.h>

namespace Greet{
  class Sprite
  {
    protected:
      Ref<Texture2D> texture;
      Vec2f texPos;
      Vec2f texSize;
    public:
      Sprite()
        : texture(nullptr), texPos(Vec2f(0, 0)), texSize(Vec2f(1, 1))
      {}

      Sprite(const Ref<Texture2D>& texture)
        : texture{texture}, texPos{Vec2f{0, 0}}, texSize{Vec2f{1, 1}}
      {}

      Sprite(const Ref<Texture2D>& texture, const Vec2f& texPos, const Vec2f& texSize)
        : texture{texture}, texPos{texPos}, texSize{texSize}
      {}

      virtual bool Update(float timeElapsed) { return false; }

      const Ref<Texture2D>& GetTexture() const { return texture; }

      Sprite* FromSpriteSheet(const Vec2f& texPos, const Vec2f& texSize) const
      {
        return new Sprite(texture, texPos + texPos * texSize, texSize * texSize);
      }

      virtual const Vec2f& GetTexPos() const { return texPos; }
      virtual const Vec2f& GetTexSize() const { return texSize; }
  };
}
