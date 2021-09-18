#include "Animation.h"

namespace Greet {

  Animation::Animation(const Ref<Texture2D>& texture, const Vec2f& texPos, const Vec2f& texSize, float frameTimer, uint32_t images, bool horizontalRead)
    : Sprite(texture, texPos, texSize), frameTimer(frameTimer), frameTime(frameTimer), images(images), image(0)
  {
    add = Vec2f(horizontalRead ? texSize.x : 0.0f, horizontalRead ? 0.0f : texSize.y);
    texPos2 = texPos;
  }

  Animation::Animation(const Sprite& sprite, float frameTimer, uint32_t images, bool horizontalRead)
    : Sprite(sprite), frameTimer(frameTimer), frameTime(frameTimer), images(images), image(0)
  {
    add = Vec2f(horizontalRead ? texSize.x : 0.0f, horizontalRead ? 0.0f : texSize.y);
    texPos2 = texPos;
  }

  bool Animation::Update(float timeElapsed)
  {
    frameTime -= timeElapsed;
    if (frameTime <= 0)
    {
      image++;
      image %= images;
      frameTime = frameTimer + frameTime;
      texPos2 = texPos + add * image;
      return true;
    }
    return false;
  }

  void Animation::SetTexPos(const Vec2f& avTexSize)
  {
    texSize = avTexSize;
  }
}
