#pragma once

#include <math/Mat4.h>
#include <math/Vec3.h>
#include <utils/MetaFile.h>

namespace Greet
{
  struct Transform3DComponent
  {
    Mat4 transform;

    Transform3DComponent(const Mat4& transform);
    Transform3DComponent(const Vec3f& pos, const Vec3f& scale, const Vec3f& rot);
    Transform3DComponent(const MetaFileClass& metaClass);
  };
}
