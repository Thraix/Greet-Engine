#include "Skybox.h"

#include <graphics/models/MeshFactory.h>
#include <graphics/shaders/ShaderFactory.h>
#include <graphics/RenderCommand.h>


namespace Greet {

  Skybox::Skybox(const Ref<CubeMap>& cubemap)
    : Skybox{cubemap, ShaderFactory::ShaderSkybox()}
  {
  }

  Skybox::Skybox(const Ref<CubeMap>& map, const Ref<Shader>& shader)
    : m_map(map), m_shader(shader)
  {
    MeshData data{MeshFactory::Cube()};
    m_mesh = NewRef<Mesh>(data);
    m_mesh->SetClockwiseRender(true);
  }

  Skybox::~Skybox()
  {}

  void Skybox::Render(const Ref<Camera3D>& camera) const
  {
    RenderCommand::EnableDepthTest(false);
    m_shader->Enable();
    camera->SetShaderUniforms(m_shader);
    m_map->Enable(0);
    m_mesh->Bind();
    m_mesh->Render();
    m_mesh->Unbind();
    m_map->Disable();
    m_shader->Disable();
    RenderCommand::ResetDepthTest();
  }
}
