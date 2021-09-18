#pragma once

#include <event/Event.h>

namespace Greet {

  class KeyEvent : public Event
  {
    protected:
      uint32_t button;
    protected:
      KeyEvent(uint32_t button)
        : button(button) {}
    public:
      uint32_t GetButton() const { return button; }
      uint32_t GetCategory() const {return INPUT_EVENT | KEYBOARD_EVENT;}
  };

  class KeyPressEvent : public KeyEvent
  {
    protected:
      uint32_t repeat;
    public:
      KeyPressEvent(uint32_t button, uint32_t repeat)
        : KeyEvent(button), repeat{repeat} {}
      EventType GetType() const {return EventType::KEY_PRESS;}
      bool IsRepeat() const { return repeat != 0; }
      uint32_t GetRepeat() const { return repeat; }
  };

  class KeyReleaseEvent : public KeyEvent
  {
    public:
      KeyReleaseEvent(uint32_t button)
        : KeyEvent(button) {}
      EventType GetType() const {return EventType::KEY_RELEASE;}
  };

  class KeyTypeEvent : public KeyEvent
  {
    private:
      uint32_t charCode;

    public:
      KeyTypeEvent(uint32_t charCode) : KeyEvent(charCode) {}
      EventType GetType() const {return EventType::KEY_TYPE;}
  };

}
