#pragma once

#include "TransformGizmo.h"
#include <common/Memory.h>

namespace Greet
{
  class BoundingBox;
  class Mat4;
  class Mesh;
  class Shader;
}

class TranslationGizmo : public TransformGizmo
{
  enum class InputType
  {
    XAxis = 0, YAxis = 1, ZAxis = 2, XPlane= 3, YPlane = 4, ZPlane = 5, FreeMove = 6, None
  };

  private:
    Greet::Ref<Greet::Mesh> axisMesh;
    Greet::Ref<Greet::Mesh> planeMesh;
    Greet::Ref<Greet::Mesh> cubeMesh;
    Greet::Ref<Greet::Shader> shader;

    InputType inputType = InputType::None;
    Greet::Vec3f pressedEntityPos;
    Greet::Vec3f pressedTranslationPos;

    std::vector<std::pair<Greet::BoundingBox, InputType>> boundingBoxes;

  public:

    TranslationGizmo();
    void Render(const Greet::Camera3DComponent& camera) const override;
    bool OnEvent(Greet::Event& event, const Greet::Camera3DComponent& cameraComponent) override;

  private:
    void RenderArrows(const Greet::Mat4& mirrorMatrix) const;
    void RenderPlanes(const Greet::Mat4& mirrorMatrix) const;
    void RenderCube() const;
    bool IsInputTypeAxis(InputType type);
    Greet::Vec3f GetAxisVector(InputType type);
    bool IsInputTypePlane(InputType type);
};
