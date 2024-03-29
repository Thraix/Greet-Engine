#pragma once

#include <stdint.h>

namespace Greet {

  enum class TextureFilter
  {
    NONE,
    LINEAR = 0x2601,  //GL_LINEAR
    NEAREST = 0x2600, //GL_NEAREST
  };

  enum class TextureWrap
  {
    NONE = 0,
    CLAMP_TO_EDGE = 0x812F,
    CLAMP_TO_BORDER = 0x812D,
    MIRRORED_REPEAT = 0x8370,
    REPEAT = 0x2901,
    MIRROR_CLAMP_TO_EDGE = 0x8743
  };

  enum class TextureInternalFormat
  {
    DEPTH_COMPONENT = 0x1902, // GL_DEPTH_COMPONENT
    DEPTH_STENCIL = 0x84F9,   // GL_DEPTH_STENCIL
    RED = 0x1903,             // GL_RED
    RGB = 0x1907,             // GL_RGB
    RGBA = 0x1908,            // GL_RGBA
    RGB32 = 0x8815,           // GL_RGB32F
    RGBA32 = 0x8814,          // GL_RGBA32F
  };

  struct TextureParams
  {
    TextureParams()
      : filter(TextureFilter::LINEAR), wrap(TextureWrap::CLAMP_TO_EDGE), internalFormat(TextureInternalFormat::RGBA)
    {}

    TextureParams(TextureFilter filter, TextureWrap wrap, TextureInternalFormat internalFormat)
      : filter(filter), wrap(wrap), internalFormat(internalFormat)
    {}

    TextureFilter filter;
    TextureWrap wrap;
    TextureInternalFormat internalFormat;
  };


  struct TextureDeleter final
  {
    void operator()(uint32_t* id);
  };

  class Texture
  {
    protected:
      uint32_t texId;
      uint32_t m_textureType;
    public:
      Texture(uint32_t textureType, bool generateTexture = true);
      Texture(uint32_t texId, uint32_t textureType);
      Texture();
      virtual ~Texture();

      virtual void Enable() const;
      virtual void Enable(uint32_t index) const;
      virtual void Disable() const;

      uint32_t GetTexId() const;
      bool Invalid() const { return texId == 0;};
      friend bool operator<(const Texture& tex1, const Texture& tex2)
      {
        return tex1.texId< tex2.texId;
      }
      friend bool operator==(const Texture& tex1, const Texture& tex2)
      {
        return tex1.texId == tex2.texId;
      }
  };

}
