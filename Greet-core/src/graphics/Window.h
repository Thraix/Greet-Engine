#pragma once

#include <common/Memory.h>
#include <input/Joystick.h>
#include <math/Vec2.h>

#include <vector>

#define MAX_KEYS			1024
#define MAX_MOUSEBUTTONS	32

#define BUTTON_STATE_PRESSED	0x0
#define BUTTON_STATE_RELEASED	0x1
#define BUTTON_STATE_DOWN		0x2
#define BUTTON_STATE_UP			0x3

#define JOYSTICK_STATE_DISCONNECTED 0x0
#define JOYSTICK_STATE_CONNECTED	0x1

class GLFWwindow;

namespace Greet {
  class GUI;
  typedef void(*WINDOW_RESIZE)(int width, int height);
  typedef void(*JOYSTICK_STATE)(uint32_t joystick, unsigned char state);
  typedef void(*WINDOW_FOCUS)(int state);

  class Window
  {
    private:
      static void* pointer;
      friend struct GLFWwindow;
      static std::string title;
      static uint32_t width, height;
      static GLFWwindow *window;

      static std::vector<Ptr<Joystick>> joysticks;

      static bool focus;
      static bool mouseButtonDown[MAX_MOUSEBUTTONS];
      static bool isMouseButtonDown;
      static Vec2f mousePos;
      static Vec2f mousePosPixel;
      static bool isMouseGrabbed;

    private:
      static bool Init();

      static void window_resize(GLFWwindow *window, int width, int height);
      static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
      static void key_char_callback(GLFWwindow* window, uint32_t charCode);

      static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
      static void window_focus_callback(GLFWwindow* window, int state);
      static void window_position_callback(GLFWwindow* window, int x, int y);
      static void mouse_position_callback(GLFWwindow* window, double xpos, double ypos);
      static void mouse_scroll_callback(GLFWwindow* window, double scrollX, double scrollY);
      static void joystick_callback(int joy, int event);
    public:

      static void CreateWindow(std::string title, uint32_t width, uint32_t height);
      static void DestroyWindow();

      static void Close();
      static bool Closed();

      static void Render();
      static void Update();
      static void Tick();

      static void GrabMouse(bool grab);
      static bool IsMouseGrabbed() { return isMouseGrabbed; }

      // Screen: (-1, 1)
      // Window: (0, Window size)
      static void TransformScreenToWindowPos(Vec2f& pos);
      static void TransformWindowToScreenPos(Vec2f& pos);

      inline static int GetWidth() { return width; };
      inline static int GetHeight() { return height; };
      inline static float GetAspect() { return width / (float) height; }
      static GLFWwindow* GetGLFWWindow() { return window; }
      static void SetTitle(const std::string& title);

      static Joystick* GetJoystick(uint32_t joystick);
      static bool IsJoystickConnected(uint32_t joystick);


      inline static void* getUserPointer(Window* window) { return window->pointer; }

      static void setUserPointer(Window* window, void* pointer);

  };
}
