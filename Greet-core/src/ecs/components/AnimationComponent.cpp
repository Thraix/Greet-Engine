#include "AnimationComponent.h"

#include <utils/MetaFileLoading.h>

namespace Greet
{
  AnimationComponent::AnimationComponent(int animationCount, float timeDelta)
    : animationCount{animationCount},
    keytime{timeDelta}
  {}

  AnimationComponent::AnimationComponent(const MetaFileClass& metaClass)
    : animationCount{MetaFileLoading::LoadInt(metaClass, "sprites", 1)},
    keytime{MetaFileLoading::LoadFloat(metaClass, "keytime", 1.0f)}
  {}
}
