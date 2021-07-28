#pragma once

#include <graphics/shaders/Shader.h>

namespace Greet {

  struct ShaderFactory
  {
    static const char* shaderErrorVert;
    static const char* shaderErrorFrag;

    static Ref<Shader> Shader2D();
    static Ref<Shader> Shader3D();
    static Ref<Shader> ShaderGUI();
    static Ref<Shader> ShaderSkybox();
    static Ref<Shader> ShaderThreshold();
    static Ref<Shader> ShaderBlur();
    static Ref<Shader> ShaderAdditive();
    static Ref<Shader> ShaderError();

  };
}
