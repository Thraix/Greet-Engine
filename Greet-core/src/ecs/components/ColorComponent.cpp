#include "ColorComponent.h"

#include <utils/MetaFileLoading.h>
#include <utils/LogUtils.h>

namespace Greet
{
  ColorComponent::ColorComponent(uint64_t color)
    : color{color}
  {}

  ColorComponent::ColorComponent(const MetaFileClass& metaClass)
    : color{MetaFileLoading::LoadHex64(metaClass, "color", 0xffff00ff)}
  {}

  MetaFile& operator<<(MetaFile& metaFile, const ColorComponent& component)
  {
    MetaFileClass meta;
    meta.AddValue("color", LogUtils::DecToHex(component.color));
    metaFile.AddMetaClass("ColorComponent", meta);
    return metaFile;
  }
}
