#pragma once

#include <common/Memory.h>
#include <graphics/renderers/Renderer2D.h>
#include <math/Vec2.h>

namespace Greet {
  class Renderable
  {
    public:

      bool render = true;

      Renderable()
      {

      }

      virtual ~Renderable()
      {

      }
      virtual void Begin(const Ref<Renderer2D>& renderer) const {}
      virtual void Render(const Ref<Renderer2D>& renderer) const = 0;
      virtual void End(const Ref<Renderer2D>& renderer) const {}

      virtual bool Update(float timeElapsed) = 0;

      virtual void SetColor(uint32_t color) = 0;
      virtual inline uint32_t GetColor() const = 0;
      virtual void SetPosition(const Vec2f& pos) = 0;
      virtual inline const Vec2f& GetPosition() const = 0;
      virtual void SetSize(const Vec2f& size) = 0;
      virtual inline const Vec2f& GetSize() const = 0;
  };
}
