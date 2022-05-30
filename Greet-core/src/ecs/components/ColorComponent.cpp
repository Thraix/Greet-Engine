#include "ColorComponent.h"

#include <utils/MetaFileLoading.h>

namespace Greet
{
  ColorComponent::ColorComponent(uint64_t color)
    : color{color}
  {}

  ColorComponent::ColorComponent(const MetaFileClass& metaClass)
    : color{MetaFileLoading::LoadHex64(metaClass, "color", 0xffff00ff)}
  {}
}
