#pragma once

#include <graphics/shaders/Shader.h>
#include <utils/MetaFile.h>

namespace Greet
{
  struct Environment2DComponent
  {
    Ref<Shader> shader;
    std::string shaderPath; // Stored to be able to be serialized, might look for a better solution at some point

    Environment2DComponent(const Ref<Shader>& shader);
    Environment2DComponent(const MetaFileClass& metaClass);

    friend MetaFile& operator<<(MetaFile& metaFile, const Environment2DComponent& component);
  };
}
