#pragma once

#include <event/Event.h>
#include <math/Vec2.h>

namespace Greet
{
  class ViewportResizeEvent : public Event
  {
    protected:
      Vec2f pos;
      Vec2f size;
    public:
      ViewportResizeEvent(float x, float y, float width, float height)
        : pos{x,y}, size{width, height}
      {}

      ViewportResizeEvent(const Vec2f& pos, const Vec2f& size)
        : pos{pos}, size{size}
      {}

      float GetX() const { return pos.w; }
      float GetY() const { return pos.h; }
      float GetWidth() const { return size.w; }
      float GetHeight() const { return size.h; }
      float GetAspect() const { return size.w / size.h; }
      const Vec2f& GetPosition() const { return pos; }
      const Vec2f& GetSize() const { return size; }

      uint32_t GetCategory() const {return VIEWPORT_EVENT;}
      EventType GetType() const {return EventType::VIEWPORT_RESIZE;}
  };
}
