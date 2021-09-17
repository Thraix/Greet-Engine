#include "Material.h"

#include <graphics/textures/TextureManager.h>
#include <utils/UUID.h>

namespace Greet {

  Material::Material(const Ref<Shader>& shader, const Ref<Texture2D>& texture)
    : m_shader{shader}, m_texture{texture}, color{Color{1,1,1,1}}, uuid{UUID::GetInstance().GetUUID()}
  {
    UpdateTexture();
  }

  Material::Material(const Ref<Shader>& shader)
    : m_shader{shader}, m_texture{nullptr}, color{Color{1,1,1,1}}, uuid{UUID::GetInstance().GetUUID()}
  {
    UpdateTexture();
  }

  void Material::Bind() const
  {
    m_shader->Enable();
    m_shader->SetUniform1f("uSpecularStrength", specularStrength);
    m_shader->SetUniform1f("uSpecularExponent", specularExponent);
    m_shader->SetUniform1f("uDiffuseStrength", diffuseStrength);
    m_shader->SetUniform1f("uAmbient", ambient);
    m_shader->SetUniform1f("uIntensity", intensity);
    m_shader->SetUniformColor4("uMaterialColor", color);
    if(m_texture)
      m_texture->Enable();
  }

  void Material::Unbind() const
  {
    if(m_texture)
      m_texture->Disable();
    m_shader->Disable();
  }

  void Material::SetShader(const Ref<Shader>& shader)
  {
    m_shader = shader;
    UpdateTexture();
  }

  void Material::UpdateTexture()
  {
    m_shader->Enable();
    m_shader->SetUniformBoolean("uHasTexture", m_texture.get() != nullptr);
    m_shader->Disable();
  }
}
