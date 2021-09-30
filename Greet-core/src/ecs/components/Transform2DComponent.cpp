#include "Transform2DComponent.h"

#include <utils/MetaFileLoading.h>

namespace Greet
{
  Transform2DComponent::Transform2DComponent(const Mat3& transform)
    : transform{transform}
  {}

  Transform2DComponent::Transform2DComponent(const Vec2f& pos, const Vec2f& scale, float rot)
    : transform{Mat3::TransformationMatrix(pos, scale, rot)}
  {}

  Transform2DComponent::Transform2DComponent(const MetaFileClass& metaClass)
    : transform{Mat3::TransformationMatrix(
        MetaFileLoading::LoadVec2f(metaClass, "position", {0, 0}),
        MetaFileLoading::LoadVec2f(metaClass, "scale", {1, 1}),
        Math::ToRadians(MetaFileLoading::LoadFloat(metaClass, "rotation", 0.0f)))}
  {}
}
