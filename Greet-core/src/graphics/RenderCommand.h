#pragma once

#include <math/Vec2.h>
#include <math/Vec4.h>
#include <graphics/Color.h>

#include <stack>

namespace Greet
{
  enum class CullFaceDirection
  {
    CW, CCW
  };

  class RenderCommand
  {

    private:
      RenderCommand() = delete;
      static std::stack<Vec4f> viewportStack;
      static Color clearColor;
    public:
      static void Init();
      /*
       Values relative to the current viewport
       position is from the top-left of the screen
       */
      static void PushViewportStack(float x, float y, float width, float height, bool ignoreParent=false);
      static void PushViewportStack(const Vec2f& pos, const Vec2f& size, bool ignoreParent=false);
      static void PushViewportStack(const Vec4f& viewport, bool ignoreParent=false);
      static void PushViewportDefaultStack();

      static void PopViewportStack();
      static Vec4f TopViewportStack();
      static void ResetViewport();

      static float GetViewportX();
      static float GetViewportY();
      static float GetViewportWidth();
      static float GetViewportHeight();
      static float GetViewportAspect();

      static void SetClearColor(const Color& clearColor);
      static const Color& GetClearColor();
      static void Clear();
      static void ClearDepthBuffer();

      static void EnableCulling(bool enabled);
      static void SetCullFace(CullFaceDirection face);
      static void ResetCulling();

      static void EnableDepthTest(bool enabled);
      static void ResetDepthTest();

      static void SetLineWidth(int lineWidth);
      static void ResetLineWidth();
  };
}

