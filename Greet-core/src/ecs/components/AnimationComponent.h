#pragma once

#include <math/Vec2.h>
#include <utils/MetaFile.h>

namespace Greet
{
  struct AnimationComponent
  {
    int aminationIndex = 0;
    int animationCount;
    float timeElapsed = 0.0f;
    float keytime;

    AnimationComponent(int animationCount, float keytime);
    AnimationComponent(const MetaFileClass& metaClass);
  };
}
