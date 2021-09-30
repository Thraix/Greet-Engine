#pragma once

#include <math/Vec2.h>
#include <math/Mat3.h>
#include <utils/MetaFile.h>

namespace Greet
{
  struct Transform2DComponent
  {
    Mat3 transform;
    Transform2DComponent(const Mat3& transform);
    Transform2DComponent(const Vec2f& pos, const Vec2f& scale, float rot);
    Transform2DComponent(const MetaFileClass& metaClass);
  };
}
