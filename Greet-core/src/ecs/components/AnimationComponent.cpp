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

  MetaFile& operator<<(MetaFile& metaFile, const AnimationComponent& animationComponent)
  {
    MetaFileClass meta;
    meta.AddValue("sprite", std::to_string(animationComponent.animationCount));
    meta.AddValue("keytime", std::to_string(animationComponent.keytime));
    metaFile.AddMetaClass("AnimationComponent", meta);
    return metaFile;
  }
}
