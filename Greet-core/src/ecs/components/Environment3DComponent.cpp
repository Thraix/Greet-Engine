#include "Environment3DComponent.h"

#include <graphics/RenderCommand.h>
#include <graphics/models/MeshFactory.h>
#include <graphics/shaders/ShaderFactory.h>
#include <utils/MetaFileLoading.h>

namespace Greet
{
  Environment3DComponent::Environment3DComponent(const Ref<Shader>& skybox, const Ref<CubeMap>& skyboxTexture, const Vec3f& lightPos, const Color& lightColor, float fogNearDistance, float fogFarDistance, const Color& fogColor)
    : skyboxMesh{new Mesh{MeshFactory::Cube()}}, skybox{skybox}, skyboxTexture{skyboxTexture}, lightPos{lightPos}, lightColor{lightColor}, fogNearDistance{fogNearDistance}, fogFarDistance{fogFarDistance}, fogColor{fogColor}
  {
    skyboxMesh->SetClockwiseRender(true);
  }

  Environment3DComponent::Environment3DComponent(const Ref<Shader>& skybox, const Ref<CubeMap>& skyboxTexture)
    : skyboxMesh{new Mesh{MeshFactory::Cube()}}, skybox{skybox}, skyboxTexture{skyboxTexture}
  {
    skyboxMesh->SetClockwiseRender(true);
  }

  Environment3DComponent::Environment3DComponent(const Ref<Shader>& skybox)
    : skyboxMesh{new Mesh{MeshFactory::Cube()}}, skybox{skybox}
  {
    skyboxMesh->SetClockwiseRender(true);
  }

  Environment3DComponent::Environment3DComponent(const Ref<CubeMap>& skyboxTexture)
    : skyboxMesh{new Mesh{MeshFactory::Cube()}}, skybox{ShaderFactory::ShaderSkybox()}, skyboxTexture{skyboxTexture}
  {
    skyboxMesh->SetClockwiseRender(true);
  }

  Environment3DComponent::Environment3DComponent()
    : skyboxMesh{new Mesh{MeshFactory::Cube()}}, skybox{ShaderFactory::ShaderSkybox()}
  {
    skyboxMesh->SetClockwiseRender(true);
  }

  Environment3DComponent::Environment3DComponent(const MetaFileClass& metaClass)
    : skyboxMesh{new Mesh{MeshFactory::Cube()}},
    skybox{MetaFileLoading::LoadShader(metaClass, "skybox", "skybox")},
    skyboxTexture{MetaFileLoading::LoadCubeMap(metaClass, "skybox-texture")},
    lightPos{MetaFileLoading::LoadVec3f(metaClass, "lightPos", {30.0, 20.0, 40.0})},
    lightColor{MetaFileLoading::LoadColor(metaClass, "lightColor", {1.0, 0.96, 0.9})},
    fogColor{MetaFileLoading::LoadColor(metaClass, "fogColor", {0.125f, 0.125f, 0.125f})},
    fogNearDistance{MetaFileLoading::LoadFloat(metaClass, "fogNearDistance", 100)},
    fogFarDistance{MetaFileLoading::LoadFloat(metaClass, "fogFarDistance", 140)},
    bloomThreshold{MetaFileLoading::LoadFloat(metaClass, "bloomThreshold", 4.5)},
    bloomFalloff{MetaFileLoading::LoadFloat(metaClass, "bloomFalloff", 0.25)}
  {
    skyboxMesh->SetClockwiseRender(true);
  }

  void Environment3DComponent::Skybox(const Camera3DComponent& camera) const
  {
    RenderCommand::EnableDepthTest(false);
    skybox->Enable();
    camera.SetShaderUniforms(skybox);
    if(skyboxTexture)
      skyboxTexture->Enable(0);
    skyboxMesh->Bind();
    skyboxMesh->Render();
    skyboxMesh->Unbind();
    if(skyboxTexture)
      skyboxTexture->Disable();
    skybox->Disable();
    RenderCommand::ResetDepthTest();
  }

  void Environment3DComponent::SetShaderUniforms(const Ref<Shader>& shader) const
  {
    shader->SetUniform3f("uLightPosition", lightPos);
    shader->SetUniformColor3("uLightColor", lightColor);
    shader->SetUniform1f("uFogNearDistance", fogNearDistance);
    shader->SetUniform1f("uFogFarDistance", fogFarDistance);
    shader->SetUniformColor3("uFogColor", fogColor);
  }
}
