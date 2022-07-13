#include "TagComponent.h"

#include <logging/Log.h>

namespace Greet
{
  TagComponent::TagComponent(const std::string& tag)
    : tag{tag}
  {}

  TagComponent::TagComponent(const MetaFileClass& metaClass)
    : tag{"Invalid"}
  {
    if(metaClass.HasValue("tag"))
      tag = metaClass.GetValue("tag");
    else
      Log::Error("tag does not exist in TagComponent");
  }

  MetaFile& operator<<(MetaFile& metaFile, const TagComponent& component)
  {
    MetaFileClass meta;
    meta.AddValue("tag", component.tag);
    metaFile.AddMetaClass("TagComponent", meta);
    return metaFile;
  }
}
