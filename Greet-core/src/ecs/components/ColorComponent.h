#pragma once

#include <utils/MetaFile.h>

namespace Greet
{
  struct ColorComponent
  {
    uint64_t color;

    ColorComponent(uint64_t color);
    ColorComponent(const MetaFileClass& metaClass);
  };
}
