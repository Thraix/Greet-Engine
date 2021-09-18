#include "Texture.h"

#include <internal/GreetGL.h>

namespace Greet {

  Texture::Texture(uint32_t textureType, bool generateTexture)
    : texId{0}, m_textureType{textureType}
  {
    ASSERT(m_textureType == GL_TEXTURE_2D || m_textureType == GL_TEXTURE_CUBE_MAP, "Invalid texture enum: ", (uint32_t)m_textureType);
    if(generateTexture)
    {
      uint32_t id;
      glGenTextures(1, &id);
      texId = id;
      Enable();
    }
  }

  Texture::Texture(uint32_t texId, uint32_t textureType)
    : texId{texId}, m_textureType{textureType}
  {
    ASSERT(m_textureType == GL_TEXTURE_2D || m_textureType == GL_TEXTURE_CUBE_MAP, "Invalid texture enum");
    Enable();
  }

  Texture::Texture()
    : texId{0}, m_textureType{0}
  {
  }

  Texture::~Texture()
  {
    glDeleteTextures(1, &texId);
  }

  void Texture::Enable() const
  {
    GLCall(glBindTexture(m_textureType, texId));
  }

  void Texture::Enable(uint32_t index) const
  {
    GLCall(glActiveTexture(GL_TEXTURE0 + index));
    GLCall(glBindTexture(m_textureType, texId));
  }

  void Texture::Disable() const
  {
    GLCall(glBindTexture(m_textureType, 0));
  }

  uint32_t Texture::GetTexId() const
  {
    return texId;
  }
}
