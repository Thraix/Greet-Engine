#pragma once

#include <math/Vec2.h>
#include <utils/MetaFile.h>

namespace Greet
{
  struct AnimationComponent
  {
    int animationCount;
    float keytime;

    // Runtime data
    int aminationIndex = 0;
    float timeElapsed = 0.0f;

    AnimationComponent(const MetaFileClass& metaClass);
    AnimationComponent(int animationCount = 1, float keytime = 1.0f);

    friend MetaFile& operator<<(MetaFile& metaFile, const AnimationComponent& animationComponent);
  };
}
