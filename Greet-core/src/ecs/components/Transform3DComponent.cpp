#include "Transform3DComponent.h"

#include <utils/MetaFileLoading.h>

namespace Greet
{
  Transform3DComponent::Transform3DComponent(const Mat4& transform)
    : transform{transform}
  {}

  Transform3DComponent::Transform3DComponent(const Vec3f& pos, const Vec3f& scale, const Vec3f& rot)
    : transform{Mat4::TransformationMatrix(pos, scale, rot)}, position{pos}, scale{scale}, rotation{rot}
  {}

  Transform3DComponent::Transform3DComponent(const MetaFileClass& metaClass)
    : Transform3DComponent{
        MetaFileLoading::LoadVec3f(metaClass, "position", {0.0f}),
        MetaFileLoading::LoadVec3f(metaClass, "scale", {1.0f}),
        MetaFileLoading::LoadVec3f(metaClass, "rotation", {0.0f}).ToRadians()}
  {}

  void Transform3DComponent::SetPosition(const Vec3f& avPos)
  {
    position = avPos;
    RecalcTransform();
  }

  void Transform3DComponent::SetScale(const Vec3f& avScale)
  {
    scale = avScale;
    RecalcTransform();
  }

  void Transform3DComponent::SetRotation(const Vec3f& avRotation)
  {
    rotation = avRotation;
    RecalcTransform();
  }

  const Vec3f& Transform3DComponent::GetPosition() const
  {
    return position;
  }

  const Vec3f& Transform3DComponent::GetScale() const
  {
    return scale;
  }

  const Vec3f& Transform3DComponent::GetRotation() const
  {
    return rotation;
  }

  void Transform3DComponent::RecalcTransform()
  {
    transform = Mat4::TransformationMatrix(position, scale, rotation);
  }
}
