#pragma once

#include <common/Types.h>
#include <graphics/textures/Texture2D.h>
#include <map>

namespace Greet {

  class Framebuffer
  {
    private:
      uint width;
      uint height;
      bool hdr;
      uint fbo;
      Ref<Texture2D> colorTexture;
      uint depthBuffer;
    public:
      Framebuffer(uint width, uint height, bool hdr);
      virtual ~Framebuffer();
      void Bind();
      void Unbind();
      void Resize(uint _width, uint _height);
      const Ref<Texture2D>& GetColorTexture() const { return colorTexture; };
      uint GetWidth() const { return width; };
      uint GetHeight() const { return height; };
  };
}
