#pragma once

#include <common/Memory.h>
#include <graphics/shaders/Shader.h>

#include <string.h>

namespace Greet {
  class Light
  {
    public:
      Vec3f position;
      uint32_t color;

    public:
      Light(const Vec3f& position, uint32_t color) : position(position) , color(color) {}

      void SetToUniform(const Ref<Shader>& shader, const std::string& prefix)
      {
        shader->SetUniform3f(("u" + prefix + "Position").c_str(), position);
        Color c{color};
        shader->SetUniformColor3(("u" + prefix + "Color").c_str(), color);
      }

  };
}
