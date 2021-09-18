#pragma once

#include <graphics/textures/Texture2D.h>

#include <stdint.h>

namespace Greet {

  class Framebuffer
  {
    private:
      uint32_t width;
      uint32_t height;
      bool hdr;
      uint32_t fbo;
      Ref<Texture2D> colorTexture;
      uint32_t depthBuffer;
    public:
      Framebuffer(uint32_t width, uint32_t height, bool hdr);
      virtual ~Framebuffer();
      void Bind();
      void Unbind();
      void Resize(uint32_t _width, uint32_t _height);
      const Ref<Texture2D>& GetColorTexture() const { return colorTexture; };
      uint32_t GetWidth() const { return width; };
      uint32_t GetHeight() const { return height; };
  };
}
