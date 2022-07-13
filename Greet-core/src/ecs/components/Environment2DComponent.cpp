#include "Environment2DComponent.h"

#include <utils/MetaFileLoading.h>

namespace Greet
{
  Environment2DComponent::Environment2DComponent(const Ref<Shader>& shader)
    : shader{shader}
  {}

  Environment2DComponent::Environment2DComponent(const MetaFileClass& metaClass)
    : shader{MetaFileLoading::LoadShader(metaClass, "shader", "shader2d")},
    shaderPath{MetaFileLoading::LoadString(metaClass, "shader", "shader2d")}
  {}

  MetaFile& operator<<(MetaFile& metaFile, const Environment2DComponent& component)
  {
    MetaFileClass meta;
    meta.AddValue("shader", component.shaderPath);
    metaFile.AddMetaClass("Environment2DComponent", meta);
    return metaFile;
  }
}
