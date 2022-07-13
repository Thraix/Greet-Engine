#include "Transform2DComponent.h"

#include <utils/MetaFileLoading.h>

namespace Greet
{
  Transform2DComponent::Transform2DComponent(const Vec2f& pos, const Vec2f& scale, float rot)
    : position{pos}, scale{scale}, rotation{rot}
  {}

  Transform2DComponent::Transform2DComponent(const MetaFileClass& metaClass)
    : position{MetaFileLoading::LoadVec2f(metaClass, "position", {0, 0})},
      scale{MetaFileLoading::LoadVec2f(metaClass, "scale", {1, 1})},
      rotation{Math::ToRadians(MetaFileLoading::LoadFloat(metaClass, "rotation", 0.0f))}
  {}

  Mat3 Transform2DComponent::GetTransform() const
  {
    return Mat3::TransformationMatrix(position, scale, rotation);
  }

  MetaFile& operator<<(MetaFile& metaFile, const Transform2DComponent& component)
  {
    MetaFileClass meta;
    meta.AddValue("position", std::to_string(component.position.x) + " " + std::to_string(component.position.y));
    meta.AddValue("scale", std::to_string(component.scale.x) + " " + std::to_string(component.scale.y));
    meta.AddValue("rotation", std::to_string(component.rotation));
    metaFile.AddMetaClass("Transform2DComponent", meta);
    return metaFile;
  }
}
