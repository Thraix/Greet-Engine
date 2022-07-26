#pragma once

#include "TransformGizmo.h"
#include <common/Memory.h>

namespace Greet
{
  class BoundingBox;
  class Mat4;
  class Mesh;
  class Shader;

  class TranslationGizmo : public TransformGizmo
  {
    enum class InputType
    {
      XAxis = 0, YAxis = 1, ZAxis = 2, XPlane= 3, YPlane = 4, ZPlane = 5, FreeMove = 6, None
    };

    private:
      Ref<Mesh> axisMesh;
      Ref<Mesh> planeMesh;
      Ref<Mesh> cubeMesh;
      Ref<Shader> shader;

      InputType inputType = InputType::None;
      Vec3f pressedEntityPos;
      Vec3f pressedTranslationPos;

      std::vector<std::pair<BoundingBox, InputType>> boundingBoxes;

    public:

      TranslationGizmo();
      void Render(const Camera3DComponent& camera) const override;
      bool OnEvent(Event& event, const Camera3DComponent& cameraComponent) override;

    private:
      void RenderArrows(const Mat4& mirrorMatrix) const;
      void RenderPlanes(const Mat4& mirrorMatrix) const;
      void RenderCube() const;
      bool IsInputTypeAxis(InputType type);
      Vec3f GetAxisVector(InputType type);
      bool IsInputTypePlane(InputType type);
  };
}
