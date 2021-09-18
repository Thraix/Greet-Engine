#pragma once
#if 0
#include <graphics/particle.h>
#include <list>

namespace Greet {
  class ParticlePool : public Renderable2D
  {
    private:
      std::list<Particle*> m_free;
      std::list<Particle*> m_used;
      uint32_t m_usedParticles;
      unsigned const int m_particles;
    public:
      ParticlePool(int particles);
      ~ParticlePool();
      bool create(Vec2 pos, Vec2 size, Vec2 vel, Vec2 acc, uint32_t color, float minspeed, float maxspeed, float frameTime, Sprite* sprite);
      bool update(float elapsedTime);
      void submit(Renderer2D* renderer) const override;
      uint32_t usedParticles() const;
      uint32_t freeParticles() const;
  };
}

#endif
