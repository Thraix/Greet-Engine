#pragma once

#include <graphics/models/Material.h>
#include <utils/MetaFile.h>

namespace Greet
{
  struct MaterialComponent
  {
    Material material;

    MaterialComponent(const Material& material);
    MaterialComponent(const MetaFileClass& metaClass);
  };
}
