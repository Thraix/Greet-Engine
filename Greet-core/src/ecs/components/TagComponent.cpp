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
}
