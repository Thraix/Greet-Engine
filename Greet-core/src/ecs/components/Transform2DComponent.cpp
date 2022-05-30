#include "Transform2DComponent.h"

#include <utils/MetaFileLoading.h>

namespace Greet
{
  Transform2DComponent::Transform2DComponent(const Mat3& transform)
    : transform{transform}
  {}

  Transform2DComponent::Transform2DComponent(const Vec2f& pos, const Vec2f& scale, float rot)
    : transform{Mat3::TransformationMatrix(pos, scale, rot)}, position{pos}, scale{scale}, rotation{rot}
  {}

  Transform2DComponent::Transform2DComponent(const MetaFileClass& metaClass)
    : position{MetaFileLoading::LoadVec2f(metaClass, "position", {0, 0})},
      scale{MetaFileLoading::LoadVec2f(metaClass, "scale", {1, 1})},
      rotation{Math::ToRadians(MetaFileLoading::LoadFloat(metaClass, "rotation", 0.0f))}
  {
    RecalcTransform();
  }

  void Transform2DComponent::SetPosition(const Vec2f& avPos)
  {
    position = avPos;
    RecalcTransform();
  }

  void Transform2DComponent::SetScale(const Vec2f& avScale)
  {
    scale = avScale;
    RecalcTransform();
  }

  void Transform2DComponent::SetRotation(float avRotation)
  {
    rotation = avRotation;
    RecalcTransform();
  }

  const Vec2f& Transform2DComponent::GetPosition() const
  {
    return position;
  }

  const Vec2f& Transform2DComponent::GetScale() const
  {
    return scale;
  }

  float Transform2DComponent::GetRotation() const
  {
    return rotation;
  }

  void Transform2DComponent::RecalcTransform()
  {
    transform = Mat3::TransformationMatrix(position, scale, rotation);
  }
}
