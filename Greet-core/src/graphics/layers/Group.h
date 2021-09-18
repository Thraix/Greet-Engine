#pragma once

#include <graphics/Renderable.h>
#include <graphics/renderers/Renderer2D.h>
#include <math/Mat3.h>
#include <math/Vec2.h>

#define __PRETTY_FUNCTION__ __func__

namespace Greet {
  class Group : public Renderable
  {
    protected:
      std::vector<Renderable*> m_renderables;
      Mat3 m_transformationMatrix;
    public:
      bool enable;
    public:
      Group(const Vec2f& position);
      Group(const Mat3& transform);
      virtual ~Group();
      virtual void Add(Renderable* renderable);
      virtual void Remove(Renderable* renderable);
      virtual bool Update(float timeElapsed) override;
      virtual void Begin(const Ref<Renderer2D>& renderer) const override;
      virtual void Render(const Ref<Renderer2D>& renderer) const override;
      virtual void End(const Ref<Renderer2D>& renderer) const override;

      void SetEnable(bool enable) { Group::enable = enable;}
      void SetTransformationMatrix(const Mat3& transformationMatrix) {m_transformationMatrix = transformationMatrix;}

      void SetColor(uint32_t color) override { Log::Warning("THIS DOES NOTHING",__PRETTY_FUNCTION__); }
      inline uint32_t GetColor() const override { Log::Warning("THIS DOES NOTHING",__PRETTY_FUNCTION__);return 0xffffffff;}
      void SetPosition(const Vec2f& position) override { Log::Warning("THIS DOES NOTHING",__PRETTY_FUNCTION__); }
      inline const Vec2f& GetPosition() const override
      {
        Log::Warning("THIS DOES NOTHING",__PRETTY_FUNCTION__);
        static Vec2f ret(0,0);
        return ret;
      }
      void SetSize(const Vec2f& size) override { Log::Warning("THIS DOES NOTHING",__PRETTY_FUNCTION__); }
      inline const Vec2f& GetSize() const override
      {
        Log::Warning("THIS DOES NOTHING",__PRETTY_FUNCTION__);
        static Vec2f ret(1,1);
        return ret;
      }
  };

}
