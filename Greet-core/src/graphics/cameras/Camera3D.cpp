#include "Camera3D.h"

namespace Greet
{
  Camera3D::Camera3D(float fov, float near, float far) :
    projectionMatrix{Mat4::PerspectiveViewport(fov, near, far)},
    fov{fov}, near{near}, far{far}
  {}

  const Mat4& Camera3D::GetViewMatrix() const { return viewMatrix;}
  const Mat4& Camera3D::GetProjectionMatrix() const { return projectionMatrix; }

  void Camera3D::SetProjectionMatrix(const Mat4& amProjectionMatrix)
  {
    projectionMatrix = amProjectionMatrix;
    invPVMatrix = ~(projectionMatrix * viewMatrix);
  }

  void Camera3D::SetViewMatrix(const Mat4& amViewMatrix)
  {
    Mat4 invMatrix = ~amViewMatrix;
    cameraPos = Vec3f{invMatrix.columns[3]};
    viewMatrix = amViewMatrix;
    invPVMatrix = ~(projectionMatrix * viewMatrix);
  }

  void Camera3D::SetShaderUniforms(const Ref<Shader>& shader) const
  {
    shader->Enable();
    shader->SetUniform3f("uCameraPos", cameraPos);
    shader->SetUniformMat4("uViewMatrix", viewMatrix);
    shader->SetUniformMat4("uProjectionMatrix", projectionMatrix);
    shader->SetUniformMat4("uInvPVMatrix", invPVMatrix);
  }

  void Camera3D::Update(float timeElapsed) {}
  void Camera3D::ViewportResize(ViewportResizeEvent& event)
  {
    projectionMatrix = Mat4::Perspective(event.GetAspect(), fov, near, far);
  }

  void Camera3D::OnEvent(Event& event) {}

  Vec3f Camera3D::GetWorldToScreenCoordinate(const Vec3f& coordinate) const
  {
    return projectionMatrix * (viewMatrix * coordinate);
  }

  Line Camera3D::GetScreenToWorldCoordinate(const Vec2f& screenPos) const
  {
    Line line;

    line.pos = invPVMatrix * Vec3f(screenPos.x, screenPos.y, -1.0);
    Vec3f far = invPVMatrix * Vec3f(screenPos.x, screenPos.y, 1.0);
    line.dir = far - line.pos;
    return line;
  }
}
