#include "Camera2DComponent.h"

#include <utils/MetaFileLoading.h>

namespace Greet
{
  Camera2DComponent::Camera2DComponent(const Vec2f& pos, const Vec2f& scale, float rotation, bool active)
    : active{active}, projectionMatrix{Mat3::OrthographicViewport()}, position{pos}, scale{scale}, rotation{rotation}
  {}

  Camera2DComponent::Camera2DComponent(const MetaFileClass& metaClass)
    : active{MetaFileLoading::LoadBool(metaClass, "active", false)},
    position{MetaFileLoading::LoadVec2f(metaClass, "position", {0, 0})},
    scale{MetaFileLoading::LoadVec2f(metaClass, "scale", {1, 1})},
    rotation{MetaFileLoading::LoadFloat(metaClass, "rotation", 0.0f)},
    projectionMatrix{Mat3::OrthographicViewport()}
  {}

  Mat3 Camera2DComponent::GetViewMatrix() const
  {
    return Mat3::InverseTransformationMatrix(position, scale, rotation);
  }

  const Mat3& Camera2DComponent::GetProjectionMatrix() const
  {
    return projectionMatrix;
  }

  Mat3 Camera2DComponent::GetPVMatrix() const
  {
    return projectionMatrix * GetViewMatrix();
  }

  Mat3 Camera2DComponent::GetInversePVMatrix() const
  {
    return ~(projectionMatrix * GetViewMatrix());
  }

  void Camera2DComponent::SetProjectionMatrix(const Mat3& amProjectionMatrix)
  {
    projectionMatrix = amProjectionMatrix;
  }

  void Camera2DComponent::SetShaderUniforms(const Ref<Shader>& shader) const
  {
    shader->SetUniform2f("uCameraPos", position);
    shader->SetUniformMat3("uViewMatrix", GetViewMatrix());
    shader->SetUniformMat3("uProjectionMatrix", projectionMatrix);
  }

  void Camera2DComponent::ViewportResize(ViewportResizeEvent& event)
  {
    SetProjectionMatrix(Mat3::OrthographicViewport());
  }

  MetaFile& operator<<(MetaFile& metaFile, const Camera2DComponent& component)
  {
    MetaFileClass meta;
    meta.AddValue("position", std::to_string(component.position.x) + " " + std::to_string(component.position.y));
    meta.AddValue("scale", std::to_string(component.scale.x) + " " + std::to_string(component.scale.y));
    meta.AddValue("rotation", std::to_string(component.rotation));
    meta.AddValue("active", component.active ? "true" : "false");
    metaFile.AddMetaClass("Camera2DComponent", meta);
    return metaFile;
  }
}
