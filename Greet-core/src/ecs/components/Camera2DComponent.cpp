#include "Camera2DComponent.h"

#include <utils/MetaFileLoading.h>

namespace Greet
{
  Camera2DComponent::Camera2DComponent(const Mat3& viewMatrix, bool active)
    : active{active}, projectionMatrix{Mat3::OrthographicViewport()}, viewMatrix{viewMatrix}
  {}

  Camera2DComponent::Camera2DComponent(const MetaFileClass& metaClass)
    : active{MetaFileLoading::LoadBool(metaClass, "active", false)},
    projectionMatrix{Mat3::OrthographicViewport()},

    viewMatrix{~Mat3::TransformationMatrix(
        MetaFileLoading::LoadVec2f(metaClass, "position", {0, 0}),
        MetaFileLoading::LoadVec2f(metaClass, "scale", {1, 1}),
        MetaFileLoading::LoadFloat(metaClass, "rotation", 0.0f)
        )}
  {}

  const Mat3& Camera2DComponent::GetViewMatrix() const { return viewMatrix; }
  const Mat3& Camera2DComponent::GetProjectionMatrix() const { return projectionMatrix; }

  void Camera2DComponent::SetProjectionMatrix(const Mat3& amProjectionMatrix)
  {
    projectionMatrix = amProjectionMatrix;
  }

  void Camera2DComponent::SetViewMatrix(const Mat3& amViewMatrix)
  {
    Mat3 invMatrix = ~viewMatrix;
    cameraPos = Vec2f{invMatrix.columns[2]};
    viewMatrix = amViewMatrix;
  }

  void Camera2DComponent::SetShaderUniforms(const Ref<Shader>& shader) const
  {
    shader->SetUniform2f("uCameraPos", cameraPos);
    shader->SetUniformMat3("uViewMatrix", viewMatrix);
    shader->SetUniformMat3("uProjectionMatrix", projectionMatrix);
  }

  void Camera2DComponent::ViewportResize(ViewportResizeEvent& event)
  {
    SetProjectionMatrix(Mat3::OrthographicViewport());
  }
}
