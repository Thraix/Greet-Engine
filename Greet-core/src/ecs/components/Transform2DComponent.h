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
      Transform2DComponent(const Mat3& transform);
      Transform2DComponent(const Vec2f& pos, const Vec2f& scale, float rot);
      Transform2DComponent(const MetaFileClass& metaClass);

      Mat3 GetTransform() const;

      friend MetaFile& operator<<(MetaFile& metaFile, const Transform2DComponent& component);
  };
}
