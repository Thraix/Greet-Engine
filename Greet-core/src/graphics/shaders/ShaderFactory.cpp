#include "ShaderFactory.h"

#include <graphics/shaders/Shader.h>

namespace Greet {

  const char* shader2d =
#include "factory/shader2d.glsl"
    ;

  const char* shader2dUI =
#include "factory/shader2dUI.glsl"
    ;

  const char* shader3d =
#include "factory/shader3d.glsl"
    ;

  const char* shaderGUI =
#include "factory/shaderGUI.glsl"
    ;

  const char* shaderSkybox =
#include "factory/shaderSkybox.glsl"
    ;

  // Bloom shaders
  const char* shaderThreshold =
#include "factory/shaderThreshold.glsl"
    ;
  const char* shaderBlur =
#include "factory/shaderBlur.glsl"
    ;
  const char* shaderAdditive =
#include "factory/shaderAdditive.glsl"
    ;
  const char* shaderTonemapping=
#include "factory/shaderTonemapping.glsl"
    ;

  const char* ShaderFactory::shaderErrorVert =
#include "factory/shaderError.vert"
    ;
  const char* ShaderFactory::shaderErrorFrag =
#include "factory/shaderError.frag"
    ;

  Ref<Shader> ShaderFactory::Shader2D()
  {
    return Shader::FromSource(shader2d);
  }

  Ref<Shader> ShaderFactory::Shader2DUI()
  {
    return Shader::FromSource(shader2dUI);
  }

  Ref<Shader> ShaderFactory::Shader3D()
  {
    return Shader::FromSource(shader3d);
  }

  Ref<Shader> ShaderFactory::ShaderGUI()
  {
    return Shader::FromSource(shaderGUI);
  }

  Ref<Shader> ShaderFactory::ShaderSkybox()
  {
    return Shader::FromSource(shaderSkybox);
  }

  Ref<Shader> ShaderFactory::ShaderThreshold()
  {
    return Shader::FromSource(shaderThreshold);
  }

  Ref<Shader> ShaderFactory::ShaderBlur()
  {
    return Shader::FromSource(shaderBlur);
  }

  Ref<Shader> ShaderFactory::ShaderAdditive()
  {
    return Shader::FromSource(shaderAdditive);
  }

  Ref<Shader> ShaderFactory::ShaderTonemapping()
  {
    return Shader::FromSource(shaderTonemapping);
  }

  Ref<Shader> ShaderFactory::ShaderError()
  {
    Log::Info("Loading error shader");
    return Shader::FromSource(shaderErrorVert, shaderErrorFrag);
  }
}
