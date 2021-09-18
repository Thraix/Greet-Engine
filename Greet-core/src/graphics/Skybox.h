#pragma once

#include <common/Memory.h>
#include <graphics/cameras/Camera3D.h>
#include <graphics/models/Mesh.h>
#include <graphics/shaders/Shader.h>
#include <graphics/textures/CubeMap.h>

namespace Greet {

  class Skybox
  {
    private:
      Ref<CubeMap> m_map;
      Ref<Shader> m_shader;
      Ref<Mesh> m_mesh;
    public:
      Skybox(const Ref<CubeMap>& map);
      Skybox(const Ref<CubeMap>&, const Ref<Shader>& shader);
      virtual ~Skybox();

      const Ref<CubeMap>& GetCubeMap() const { return m_map; };
      void Render(const Ref<Camera3D>& camera) const;
  };
}
