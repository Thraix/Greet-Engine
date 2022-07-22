#pragma once

#include <math/Vec2.h>
#include <math/Mat3.h>
#include <utils/MetaFile.h>

namespace Greet
{
  struct Transform2DComponent
  {
    public:
      Vec2f position{0, 0};
      Vec2f scale{1, 1};
      float rotation{0};

    public:
      Transform2DComponent(const Vec2f& pos = Vec2f{0, 0}, const Vec2f& scale = Vec2f{1, 1}, float rot = 0);
      Transform2DComponent(const MetaFileClass& metaClass);

      Mat3 GetTransform() const;
      Mat3 GetInverseTransform() const;

      friend MetaFile& operator<<(MetaFile& metaFile, const Transform2DComponent& component);
  };
}
