#include "Transform3DComponent.h"

#include <utils/MetaFileLoading.h>

namespace Greet
{
  Transform3DComponent::Transform3DComponent(const Mat4& transform)
    : transform{transform}
  {}

  Transform3DComponent::Transform3DComponent(const Vec3f& pos, const Vec3f& scale, const Vec3f& rot)
    : transform{Mat4::TransformationMatrix(pos, scale, rot)}
  {}

  Transform3DComponent::Transform3DComponent(const MetaFileClass& metaClass)
    : transform{Mat4::TransformationMatrix(
        MetaFileLoading::LoadVec3f(metaClass, "position", {0.0f}),
        MetaFileLoading::LoadVec3f(metaClass, "scale", {1.0f}),
        MetaFileLoading::LoadVec3f(metaClass, "rotation", {0.0f}).ToRadians())}
  {}
}
