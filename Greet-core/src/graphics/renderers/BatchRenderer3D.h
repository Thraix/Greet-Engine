#pragma once

#include <common/Memory.h>
#include <graphics/cameras/Camera3D.h>
#include <graphics/models/EntityModel.h>
#include <graphics/renderers/Renderer3D.h>

#include <map>
#include <vector>

namespace Greet {

  class BatchRenderer3D : public Renderer3D
  {
    private:
      std::map<Ref<Material>, std::vector<Ref<EntityModel>>, RefLess<Material>> m_map;
    public:
      BatchRenderer3D()
        : Renderer3D(){}

      void Submit(const Ref<EntityModel>& model);
      void Render(const Ref<Camera3D>& camera) const override;
      void Update(float timeElapsed) override;
  };
}
