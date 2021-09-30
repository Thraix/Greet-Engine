#include "MaterialComponent.h"

#include <utils/MetaFileLoading.h>

namespace Greet
{
  MaterialComponent::MaterialComponent(const Ref<Material>& material)
    : material{material}
  {}

  MaterialComponent::MaterialComponent(const MetaFileClass& metaClass)
    : material{MetaFileLoading::LoadMaterial(metaClass)}
  {}
}
