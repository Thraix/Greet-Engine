#pragma once

#include <common/Memory.h>
#include <graphics/Sprite.h>
#include <graphics/textures/Texture2D.h>
#include <math/Vec2.h>

#include <stdint.h>

namespace Greet {
  class Animation : public Sprite
  {
    private:
      Vec2f add;

      uint32_t image;
      uint32_t images;
      bool horizontalRead;
      float frameTime;
      float frameTimer;
      Vec2f texPos2;
    public:
      Animation(const Ref<Texture2D>& texture, const Vec2f& texPos, const Vec2f& texSize, float frameTimer, uint32_t images, bool horizontalRead);
      Animation(const Sprite& sprite, float frameTimer, uint32_t images, bool horizontalRead);
      virtual ~Animation() {}

      bool Update(float elapsedTime) override;

      void SetTexPos(const Vec2f& texSize);

      const Vec2f& GetTexPos() const override { return texPos2; }
  };
}
