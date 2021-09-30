#pragma once

#include <utils/MetaFile.h>

#include <string>

namespace Greet
{
  struct TagComponent
  {
    std::string tag;

    TagComponent(const std::string& tag);
    TagComponent(const MetaFileClass& metaClass);
  };
}
