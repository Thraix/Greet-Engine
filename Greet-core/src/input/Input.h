#pragma once

#include <input/InputDefines.h>
#include <math/Vec2.h>

#include <unordered_map>

namespace Greet
{
  class Input
  {
    public:
      friend class Window;
    private:
      Input() = delete;

      static Vec2f mouseLastPos;
      static Vec2f mousePos;
      static std::unordered_map<int, bool> mouseButtonsDown;
      static std::unordered_map<int, bool> keyButtonsDown;
      static float mouseScrollX;
      static float mouseScrollY;

      static void SetMousePos(const Vec2f& pos);
      static void SetKeyButton(int keyCode, bool value);
      static void SetMouseButton(int keyCode, bool value);
      static void AddMouseScroll(float scrollX, float scrollY);

      static Vec2f ConvertMousePosToViewport(const Vec2f& pos);

    public:

      static bool IsKeyDown(int keyCode);
      static bool IsMouseDown(int keyCode);
      static Vec2f GetMousePos(bool ignoreViewport = false);
      static Vec2f GetMousePosDelta(bool ignoreViewport = false);
      static Vec2f GetMousePosPixel(bool ignoreViewport = false);
      static Vec2f GetMousePosPixelDelta(bool ignoreViewport = false);
      static float GetMouseScrollX();
      static float GetMouseScrollY();

      static void Update();
  };
}
