#include "Camera3DComponent.h"

#include <utils/MetaFileLoading.h>

namespace Greet
{
  Camera3DComponent::Camera3DComponent(const Mat4& viewMatrix, float fov, float near, float far, bool active)
    :
      active{active}, fov{fov}, near{near}, far{far},
    projectionMatrix{Mat4::PerspectiveViewport(fov, near, far)},
    viewMatrix{viewMatrix},
    invPVMatrix{~(projectionMatrix * viewMatrix)}
  {}

  Camera3DComponent::Camera3DComponent(const MetaFileClass& metaClass)
    :
    active{MetaFileLoading::LoadBool(metaClass, "active", true)},
    fov{MetaFileLoading::LoadFloat(metaClass, "fov", 90.0f)},
    near{MetaFileLoading::LoadFloat(metaClass, "near", 0.01f)},
    far{MetaFileLoading::LoadFloat(metaClass, "far", 100.0f)},
    projectionMatrix{Mat4::PerspectiveViewport(fov, near, far)},
    viewMatrix{Mat4::ViewMatrix(
        MetaFileLoading::LoadVec3f(metaClass, "position", {0.0f}),
        MetaFileLoading::LoadVec3f(metaClass, "rotation", {0.0f}).ToRadians()
        )},
    invPVMatrix{~(projectionMatrix * viewMatrix)}
  {}

  const Vec3f& Camera3DComponent::GetPosition() const
  {
    return cameraPos;
  }

  const Mat4& Camera3DComponent::GetViewMatrix() const
  {
    return viewMatrix;
  }

  const Mat4& Camera3DComponent::GetProjectionMatrix() const
  {
    return projectionMatrix;
  }

  const Mat4& Camera3DComponent::GetInvPVMatrix() const
  {
    return invPVMatrix;
  }

  void Camera3DComponent::SetProjectionMatrix(const Mat4& amProjectionMatrix)
  {
    projectionMatrix = amProjectionMatrix;
    invPVMatrix = ~(projectionMatrix * viewMatrix);
  }

  void Camera3DComponent::SetViewMatrix(const Mat4& amViewMatrix)
  {
    Mat4 invMatrix = ~amViewMatrix;
    cameraPos = Vec3f{invMatrix.columns[3]};
    viewMatrix = amViewMatrix;
    invPVMatrix = ~(projectionMatrix * viewMatrix);
  }

  void Camera3DComponent::SetShaderUniforms(const Ref<Shader>& shader) const
  {
    shader->SetUniform3f("uCameraPos", cameraPos);
    shader->SetUniformMat4("uViewMatrix", viewMatrix);
    shader->SetUniformMat4("uProjectionMatrix", projectionMatrix);
    shader->SetUniformMat4("uInvPVMatrix", invPVMatrix);
  }

  void Camera3DComponent::ViewportResize(ViewportResizeEvent& event)
  {
    SetProjectionMatrix(Mat4::Perspective(event.GetAspect(), fov, near, far));
  }

  Vec3f Camera3DComponent::GetWorldToScreenCoordinate(const Vec3f& coordinate) const
  {
    return projectionMatrix * (viewMatrix * coordinate);
  }

  Vec3f Camera3DComponent::GetWorldToScreenCoordinate(const Mat4& transformation, const Vec3f& coordinate) const
  {
    return transformation * (projectionMatrix * (viewMatrix * coordinate));
  }

  Line Camera3DComponent::GetScreenToWorldCoordinate(const Vec2f& screenPos) const
  {
    Line line;
    line.pos = invPVMatrix * Vec3f(screenPos.x, screenPos.y, -1.0);
    Vec3f far = invPVMatrix * Vec3f(screenPos.x, screenPos.y, 1.0);
    line.dir = far - line.pos;
    return line;
  }

  Line Camera3DComponent::GetScreenToWorldCoordinate(const Mat4& invTransformation, const Vec2f& screenPos) const
  {
    Line line;
    line.pos = invTransformation * (invPVMatrix * Vec3f(screenPos.x, screenPos.y, -1.0));
    Vec3f far = invTransformation * (invPVMatrix * Vec3f(screenPos.x, screenPos.y, 1.0));
    line.dir = far - line.pos;
    return line;
  }
}
