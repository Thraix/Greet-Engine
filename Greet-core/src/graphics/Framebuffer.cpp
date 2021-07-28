#include "Framebuffer.h"

#include <graphics/Window.h>
#include <internal/GreetGL.h>
#include <math/Maths.h>
#include <graphics/RenderCommand.h>

namespace Greet {

  Framebuffer::Framebuffer(uint width, uint height, bool hdr)
    : width(width), height(height), hdr{hdr}
  {
    GLCall(glGenFramebuffers(1, &fbo));
    GLCall(glGenRenderbuffers(1, &depthBuffer));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

    colorTexture = Texture2D::Create(width, height, TextureParams(TextureFilter::LINEAR, TextureWrap::CLAMP_TO_EDGE, hdr ? TextureInternalFormat::RGB32 : TextureInternalFormat::RGB));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture->GetTexId(), 0));

    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height));
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer));

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
  }

  Framebuffer::~Framebuffer()
  {
    GLCall(glDeleteFramebuffers(1, &fbo));
    GLCall(glDeleteBuffers(1, &depthBuffer));
  }

  void Framebuffer::Bind()
  {
    GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo));
    GLCall(glClearColor(0, 0, 0, 0));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    RenderCommand::PushViewportStack(0, 0, width, height, true);
  }

  void Framebuffer::Unbind()
  {
    RenderCommand::PopViewportStack();
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
  }

  void Framebuffer::Resize(uint _width, uint _height)
  {
    if(width != _width || height != _height)
    {
      GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo));
      width = _width;
      height = _height;
      colorTexture = Texture2D::Create(width, height, TextureParams(TextureFilter::LINEAR, TextureWrap::CLAMP_TO_EDGE, hdr ? TextureInternalFormat::RGB32 : TextureInternalFormat::RGB));
      GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture->GetTexId(), 0));

      GLCall(glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer));
      GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height));
      GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer));
    }
  }
}
