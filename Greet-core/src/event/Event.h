#pragma once

#include <utils/Utils.h>

namespace Greet {

#define EVENT_IS_TYPE(event, type) ((event).GetType() == (type))
#define EVENT_IS_CATEGORY(event, category) ((event).GetCategory() & (category)) == (category)

  enum EventCategory : uint32_t
  {
    NONE = 0,
    WINDOW_EVENT = BIT(0),
    INPUT_EVENT = BIT(1),
    MOUSE_EVENT = BIT(2),
    KEYBOARD_EVENT = BIT(3),
    JOYSTICK_EVENT = BIT(4),
    VIEWPORT_EVENT = BIT(5),
  };

  enum class EventType : uint32_t
  {
    NONE = 0,
    WINDOW_RESIZE, WINDOW_MOVE, WINDOW_CLOSE, WINDOW_FOCUS, WINDOW_UNFOCUS,
    KEY_PRESS, KEY_RELEASE, KEY_TYPE,
    MOUSE_PRESS, MOUSE_RELEASE, MOUSE_MOVE, MOUSE_SCROLL,
    JOYSTICK_CONNECT, JOYSTICK_DISCONNECT, JOYSTICK_PRESS, JOYSTICK_RELEASE, JOYSTICK_STICK_MOVE, JOYSTICK_TRIGGER_MOVE,
    VIEWPORT_RESIZE
  };

  const uint32_t EVENT_HANDLED = BIT(0);
  const uint32_t EVENT_FOCUSED = BIT(1);
  const uint32_t EVENT_UNFOCUSED = BIT(2);

  class Event
  {
    private:
      uint32_t flags = 0;
    protected:
      Event()
      {}

    public:
      virtual ~Event() {}

      virtual EventType GetType() const = 0;
      virtual uint32_t GetCategory() const = 0;
      uint32_t GetFlags() const { return flags; }
      void AddFlag(uint32_t flag) { flags |= flag;}
  };
}
