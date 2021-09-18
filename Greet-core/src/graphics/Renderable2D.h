#pragma once

#include <common/Memory.h>
#include <graphics/Renderable.h>
#include <graphics/Sprite.h>
#include <math/Vec2.h>

namespace Greet{

  struct VertexData
  {
    Vec2f vertex;
    Vec2f texCoord;
    float texID;
    uint32_t color;
  };

  class Renderable2D : public Renderable
  {
    public:
      uint32_t m_color;
      Vec2f m_position;
      Vec2f m_size;
    protected:
      Ref<Sprite> m_sprite;
    protected:

    public:
      Renderable2D(const Vec2f& position, const Vec2f& size, uint32_t color)
        : m_position{position},m_size{size}, m_color{color}, m_sprite{nullptr}
      {}

      Renderable2D(const Vec2f& position, const Vec2f& size, uint32_t color, const Ref<Sprite>& sprite)
        : m_position{position},m_size{size}, m_color{color}, m_sprite{sprite}
      {}

      Renderable2D()
        : m_position(Vec2f(0, 0)), m_size(Vec2f(1, 1)), m_color(0xffffffff)
      {}

      void SetColor(uint32_t color) override { m_color = color; }
      inline uint32_t GetColor() const override { return m_color;}
      void SetPosition(const Vec2f& position) override { m_position = position; }
      inline const Vec2f& GetPosition() const override { return m_position;}
      void SetSize(const Vec2f& size) override { m_size = size; }
      inline const Vec2f& GetSize() const override { return m_size;}

      virtual void Render(const Ref<Renderer2D>& renderer) const override
      {
        if (render)
          renderer->Draw(*this);
      }

      virtual bool Update(float timeElapsed) override
      {
        if(m_sprite)
          return m_sprite->Update(timeElapsed);
        return false;
      }

      const Ref<Texture2D>& GetTexture() const
      {
        static Ref<Texture2D> nullTexture{nullptr};
        return m_sprite ? m_sprite->GetTexture() : nullTexture;
      }

      const Vec2f& GetTexPos() const
      {
        static Vec2f nullVec{0, 0};
        return m_sprite ? m_sprite->GetTexPos() : nullVec;
      }

      const Vec2f& GetTexSize() const
      {
        static Vec2f nullVec{1, 1};
        return m_sprite ? m_sprite->GetTexSize() : nullVec;
      }
      const Ref<Sprite>& GetSprite() const { return m_sprite; }
  };
}
