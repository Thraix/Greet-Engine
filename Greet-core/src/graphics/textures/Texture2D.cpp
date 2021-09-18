#include "Texture2D.h"

#include <internal/GreetGL.h>
#include <utils/ImageUtils.h>

namespace Greet {

  Texture2D::Texture2D(uint32_t width, uint32_t height, TextureParams params)
    : Texture(GL_TEXTURE_2D,true), width{width}, height{height}, params(params)
    {
      GenTexture();
    }

  Texture2D::Texture2D(const std::string& filename, TextureParams params)
    : Resource(filename), Texture(GL_TEXTURE_2D,true), params(params)
  {
    LoadTexture(filename);
  }

  Texture2D::Texture2D(const ImageData& imageData, TextureParams params)
    : Texture(GL_TEXTURE_2D, true), width(imageData.width), height(imageData.height), params(params)
  {
    GenTexture(imageData);
  }

  Texture2D::Texture2D(uint32_t texId, uint32_t width, uint32_t height, TextureParams params)
    : Texture(texId, (uint32_t)GL_TEXTURE_2D), width(width), height(height), params(params)
  {
  }

  Texture2D::Texture2D(TextureParams params)
    : Texture(GL_TEXTURE_2D, false), params(params)
  {
  }

  void Texture2D::LoadTexture(const std::string& filename)
  {
    auto res = ImageUtils::LoadImage(filename.c_str());
    GenTexture(res.second);
  }

  void Texture2D::GenTexture(const ImageData& imageData)
  {
    width = imageData.width;
    height = imageData.height;
    Enable();
    if(params.filter != TextureFilter::NONE)
    {
      GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  (uint32_t)params.filter));
      GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (uint32_t)params.filter));
    }
    if(params.wrap != TextureWrap::NONE)
    {
      GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  (uint32_t)params.wrap));
      GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (uint32_t)params.wrap));
    }
    uint32_t format = (uint32_t)params.internalFormat;
    if(format == GL_RGB32F) format = GL_RGB;
    if(format == GL_RGBA32F) format = GL_RGBA;
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, (uint32_t)params.internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, imageData.data.get()));
    Disable();
  }

  void Texture2D::GenTexture()
  {
    Enable();
    if(params.filter != TextureFilter::NONE)
    {
      GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  (uint32_t)params.filter));
      GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (uint32_t)params.filter));
    }
    if(params.wrap != TextureWrap::NONE)
    {
      GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  (uint32_t)params.wrap));
      GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (uint32_t)params.wrap));
    }
    uint32_t format = (uint32_t)params.internalFormat;
    if(format == GL_RGB32F) format = GL_RGB;
    if(format == GL_RGBA32F) format = GL_RGBA;
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, (uint32_t)params.internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr));
    Disable();
  }

  void Texture2D::SetPixels(const ImageData& imageData) const
  {
    if(imageData.width != width || imageData.height != height)
    {
      Log::Error("Image size does not match texture size: image=(%s, %s), texture=(%s, %s)", imageData.width, imageData.height, width, height);
      return;
    }
    Enable();
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, (uint32_t)params.internalFormat, width, height, 0, (uint32_t)params.internalFormat, GL_UNSIGNED_BYTE, imageData.data.get()));
  }

  void Texture2D::ReloadResource()
  {
    if(texId)
      LoadTexture(filePath);
  }

  Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, TextureParams params)
  {
    return Ref<Texture2D>(new Texture2D(width, height, params));
  }

  Ref<Texture2D> Texture2D::Create(const std::string& filename, TextureParams params)
  {
    return Ref<Texture2D>(new Texture2D(filename, params));
  }

  Ref<Texture2D> Texture2D::Create(const ImageData& imageData, TextureParams params)
  {
    return Ref<Texture2D>(new Texture2D(imageData, params));
  }

  Ref<Texture2D> Texture2D::Create(uint32_t texID, uint32_t width, uint32_t height, TextureParams params)
  {
    return Ref<Texture2D>(new Texture2D(texID, width, height, params));
  }

  Ref<Texture2D> Texture2D::Create(TextureParams params)
  {
    return Ref<Texture2D>(new Texture2D(params));
  }
}
