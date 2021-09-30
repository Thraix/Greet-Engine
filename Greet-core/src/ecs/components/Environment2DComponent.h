#pragma once

#include <graphics/shaders/Shader.h>
#include <utils/MetaFile.h>

namespace Greet
{
  struct Environment2DComponent
  {
    Ref<Shader> shader;

    Environment2DComponent(const Ref<Shader>& shader);
    Environment2DComponent(const MetaFileClass& metaClass);
  };
}
