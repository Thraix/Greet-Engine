#include "Environment2DComponent.h"

#include <utils/MetaFileLoading.h>

namespace Greet
{
  Environment2DComponent::Environment2DComponent(const Ref<Shader>& shader)
    : shader{shader}
  {}

  Environment2DComponent::Environment2DComponent(const MetaFileClass& metaClass)
    : shader{MetaFileLoading::LoadShader(metaClass, "shader2d")}
  {}
}
