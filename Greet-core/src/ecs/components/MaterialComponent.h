#pragma once

#include <graphics/models/Material.h>
#include <utils/MetaFile.h>

namespace Greet
{
  struct MaterialComponent
  {
    Ref<Material> material;

    MaterialComponent(const Ref<Material>& material);
    MaterialComponent(const MetaFileClass& metaClass);
  };
}
