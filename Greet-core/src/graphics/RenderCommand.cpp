#include "RenderCommand.h"

#include <graphics/Window.h>
#include <internal/GreetGL.h>
#include <logging/Log.h>

namespace Greet
{
  std::stack<Vec4f> RenderCommand::viewportStack;
  Color RenderCommand::clearColor{0,0,0,1};


  void RenderCommand::Init()
  {
    ResetCulling();
    ResetDepthTest();

    GLCall(glEnable(GL_TEXTURE_2D));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_MULTISAMPLE));
    GLCall(glProvokingVertex(GL_FIRST_VERTEX_CONVENTION));
  }

  void RenderCommand::PushViewportStack(float x, float y, float width, float height, bool ignoreParent)
  {
    PushViewportStack({x,y,width,height}, ignoreParent);
  }

  void RenderCommand::PushViewportStack(const Vec2f& pos, const Vec2f& size, bool ignoreParent)
  {
    PushViewportStack({pos.x,pos.y,size.w,size.h}, ignoreParent);
  }

  void RenderCommand::PushViewportStack(const Vec4f& viewport, bool ignoreParent)
  {
    if(ignoreParent)
    {
      GLCall(glViewport(viewport.x, viewport.y, viewport.z, viewport.w));
      viewportStack.push(viewport);
    }
    else
    {
      const Vec4f& lastViewport = viewportStack.empty()
        ? Vec4f(0, 0, Window::GetWidth(), Window::GetHeight())
        : viewportStack.top();

      Vec4 vp{viewport};
      vp.x += lastViewport.x;
      vp.y = lastViewport.y + lastViewport.w - viewport.y - viewport.w;
      GLCall(glViewport(vp.x, vp.y, vp.z, vp.w));
      viewportStack.push(vp);
    }
  }

  void RenderCommand::PushViewportDefaultStack()
  {
    Vec4f vp{0,0, (float)Window::GetWidth(), (float)Window::GetHeight()};
    GLCall(glViewport(vp.x, vp.y, vp.z, vp.w));
    viewportStack.push(vp);
  }

  void RenderCommand::PopViewportStack()
  {
    if(viewportStack.empty())
    {
      Log::Warning("Trying to pop empty viewport stack");
      return;
    }
    viewportStack.pop();

    // Use window size as default viewport if stack is empty
    const Vec4f& vp = viewportStack.empty()
      ? Vec4f(0, 0, Window::GetWidth(), Window::GetHeight())
      : viewportStack.top();

    GLCall(glViewport(vp.x, vp.y, vp.z, vp.w));
  }

  void RenderCommand::ResetViewport()
  {
    while(!viewportStack.empty())
      viewportStack.pop();

    GLCall(glViewport(0,0,Window::GetWidth(), Window::GetHeight()));
  }

  Vec4f RenderCommand::TopViewportStack()
  {
    if(viewportStack.empty())
      return Vec4f{0,0, (float)Window::GetWidth(), (float)Window::GetHeight()};
    return viewportStack.top();
  }

  float RenderCommand::GetViewportX()
  {
    if(viewportStack.empty())
      return 0;
    return viewportStack.top().x;
  }

  float RenderCommand::GetViewportY()
  {
    if(viewportStack.empty())
      return 0;
    return viewportStack.top().y;
  }

  float RenderCommand::GetViewportWidth()
  {
    if(viewportStack.empty())
      return Window::GetWidth();
    return viewportStack.top().z;
  }

  float RenderCommand::GetViewportHeight()
  {
    if(viewportStack.empty())
      return Window::GetHeight();
    return viewportStack.top().w;
  }

  float RenderCommand::GetViewportAspect()
  {
    if(viewportStack.empty())
      return Window::GetWidth() / (float)Window::GetHeight();
    return viewportStack.top().z / viewportStack.top().w;
  }

  void RenderCommand::SetClearColor(const Color& clearColor)
  {
    RenderCommand::clearColor = clearColor;
    GLCall(glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a));
  }

  const Color& RenderCommand::GetClearColor()
  {
    return clearColor;
  }

  void RenderCommand::Clear()
  {
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  }

  void RenderCommand::ClearDepthBuffer()
  {
    GLCall(glClear(GL_DEPTH_BUFFER_BIT));
  }

  void RenderCommand::EnableCulling(bool enabled)
  {
    if(enabled) { GLCall(glEnable (GL_CULL_FACE)); }
    else        { GLCall(glDisable(GL_CULL_FACE)); }
  }

  void RenderCommand::SetCullFace(CullFaceDirection face)
  {
    switch(face)
    {
      case CullFaceDirection::CCW:
        GLCall(glFrontFace(GL_CCW));
        return;
      case CullFaceDirection::CW:
        GLCall(glFrontFace(GL_CW));
        return;
    }
  }

  void RenderCommand::ResetCulling()
  {
    SetCullFace(CullFaceDirection::CCW);
    EnableCulling(true);
  }

  void RenderCommand::EnableDepthTest(bool enabled)
  {
    if(enabled) { GLCall(glEnable (GL_DEPTH_TEST)); }
    else        { GLCall(glDisable(GL_DEPTH_TEST)); }
  }

  void RenderCommand::ResetDepthTest()
  {
    EnableDepthTest(true);
  }

  void RenderCommand::SetLineWidth(int lineWidth)
  {
    glLineWidth(lineWidth);
  }

  void RenderCommand::ResetLineWidth()
  {
    glLineWidth(1);
  }
}

