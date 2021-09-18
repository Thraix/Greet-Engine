#pragma once

#include <common/Memory.h>
#include <graphics/textures/Texture.h>
#include <utils/ImageData.h>
#include <utils/Resource.h>

namespace Greet {

  class Texture2D : public Texture, public Resource
  {
    private:
      uint32_t width;
      uint32_t height;
      TextureParams params;

    private:
      Texture2D(uint32_t width, uint32_t height, TextureParams params = TextureParams());
      Texture2D(const std::string& filename, TextureParams params = TextureParams());
      Texture2D(const ImageData& bits, TextureParams params = TextureParams());
      Texture2D(uint32_t texID, uint32_t width, uint32_t height, TextureParams params = TextureParams());
      Texture2D(TextureParams params = TextureParams());

    public:

      void SetPixels(const ImageData& imageData) const;
      void ReloadResource() override;
      inline uint32_t GetWidth() const { return width; }
      inline uint32_t GetHeight() const { return height; }

      static Ref<Texture2D> Create(uint32_t width, uint32_t height, TextureParams params = TextureParams());
      static Ref<Texture2D> Create(const std::string& filename, TextureParams params = TextureParams());
      static Ref<Texture2D> Create(const ImageData& imageData, TextureParams params = TextureParams());
      static Ref<Texture2D> Create(uint32_t texID, uint32_t width, uint32_t height, TextureParams params = TextureParams());
      static Ref<Texture2D> Create(TextureParams params = TextureParams());
    private:
      void LoadTexture(const std::string& filename);
      void GenTexture(const ImageData& imageData);
      void GenTexture();

  };
}
