#include "ScaleGizmo.h"

#include <ecs/components/Camera3DComponent.h>
#include <event/MouseEvent.h>
#include <graphics/models/MeshFactory.h>
#include <graphics/shaders/Shader.h>
#include <utils/OBJUtils.h>
#include <input/InputDefines.h>

namespace Greet
{
  ScaleGizmo::ScaleGizmo()
  {
    axisMesh = NewRef<Mesh>(OBJUtils::LoadObj("res/objs/axis.obj"));
    planeMesh = NewRef<Mesh>(MeshFactory::Plane({0.8,0,0.8}, {0.2}));
    cubeMesh = NewRef<Mesh>(MeshFactory::Cube({0,0,0}, {0.4}));
    planeMesh->SetEnableCulling(false);
    axisMesh->SetEnableCulling(false);
    shader = Shader::FromFile("res/shaders/directional_light.shader");

    // Axis bounding boxes
    BoundingBox boxYAxis = axisMesh->GetBoundingBox();
    BoundingBox boxXAxis = axisMesh->GetBoundingBox();
    std::swap(boxXAxis.boundingBoxMax.x, boxXAxis.boundingBoxMax.y);
    std::swap(boxXAxis.boundingBoxMin.x, boxXAxis.boundingBoxMin.y);
    BoundingBox boxZAxis = axisMesh->GetBoundingBox();
    std::swap(boxZAxis.boundingBoxMax.z, boxZAxis.boundingBoxMax.y);
    std::swap(boxZAxis.boundingBoxMin.z, boxZAxis.boundingBoxMin.y);

    boundingBoxes.emplace_back(boxXAxis, InputType::XAxis);
    boundingBoxes.emplace_back(boxYAxis, InputType::YAxis);
    boundingBoxes.emplace_back(boxZAxis, InputType::ZAxis);

    // Plane bounding boxes
    BoundingBox boxYPlane = planeMesh->GetBoundingBox();
    BoundingBox boxXPlane = planeMesh->GetBoundingBox();
    std::swap(boxXPlane.boundingBoxMax.x, boxXPlane.boundingBoxMax.y);
    std::swap(boxXPlane.boundingBoxMin.x, boxXPlane.boundingBoxMin.y);
    BoundingBox boxZPlane = planeMesh->GetBoundingBox();
    std::swap(boxZPlane.boundingBoxMax.z, boxZPlane.boundingBoxMax.y);
    std::swap(boxZPlane.boundingBoxMin.z, boxZPlane.boundingBoxMin.y);

    boundingBoxes.emplace_back(boxXPlane, InputType::XPlane);
    boundingBoxes.emplace_back(boxYPlane, InputType::YPlane);
    boundingBoxes.emplace_back(boxZPlane, InputType::ZPlane);

    // Free move bounding box
    boundingBoxes.emplace_back(cubeMesh->GetBoundingBox(), InputType::FreeMove);
  }

  void ScaleGizmo::Render(const Camera3DComponent& camera) const
  {
    shader->Enable();
    Mat4 mirrorMatrix = Mat4::Scale({
        camera.GetPosition().x > position.x ? 1.0f : -1.0f,
        camera.GetPosition().y > position.y ? 1.0f : -1.0f,
        camera.GetPosition().z > position.z ? 1.0f : -1.0f
    });
    camera.SetShaderUniforms(shader);
    shader->SetUniform1f("uSpecularStrength", 0.0f);
    shader->SetUniform1f("uAmbient", 0.7f);
    shader->SetUniformBoolean("uHasTexture", false);
    shader->SetUniform3f("uLightDirection", mirrorMatrix * Vec3f{1,1,1});
    RenderCube();
    RenderArrows(mirrorMatrix);
    RenderPlanes(mirrorMatrix);
  }

  bool ScaleGizmo::OnEvent(Event& event, const Camera3DComponent& cameraComponent)
  {
    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent e = static_cast<MousePressEvent&>(event);
      if(e.GetButton() != GREET_MOUSE_1)
        return false;

      Mat4 mirrorMatrix = Mat4::Scale({
          cameraComponent.GetPosition().x > position.x ? 1.0f : -1.0f,
          cameraComponent.GetPosition().y > position.y ? 1.0f : -1.0f,
          cameraComponent.GetPosition().z > position.z ? 1.0f : -1.0f
      });
      Line line = cameraComponent.GetScreenToWorldCoordinate(mirrorMatrix * Mat4::Translate(-position), e.GetPosition());
      int minDistance = 100;
      inputType = InputType::None;
      for(auto&& boundingBox : boundingBoxes)
      {
        std::pair<bool, Vec3f> collision = boundingBox.first.LineIntersects(line);
        float d = (cameraComponent.GetPosition() - collision.second).Length();
        if(collision.first && (minDistance > d || (boundingBox.second == InputType::FreeMove && IsInputTypeAxis(inputType))))
        {
          minDistance = d;
          inputType = boundingBox.second;
          pressedScale = scale;
        }
      }
      if(IsInputTypeAxis(inputType))
        pressedScalePos = Line{{0,0,0}, GetAxisVector(inputType)}.PointClosestFromLine(line);
      else if(IsInputTypePlane(inputType))
        pressedScalePos = Plane{GetAxisVector(inputType), {0,0,0}}.LineIntersection(line);
      return inputType != InputType::None;
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      MouseMoveEvent e = static_cast<MouseMoveEvent&>(event);
      Mat4 mirrorMatrix = Mat4::Scale({
          cameraComponent.GetPosition().x > position.x ? 1.0f : -1.0f,
          cameraComponent.GetPosition().y > position.y ? 1.0f : -1.0f,
          cameraComponent.GetPosition().z > position.z ? 1.0f : -1.0f
      });
      Line line = cameraComponent.GetScreenToWorldCoordinate(mirrorMatrix * Mat4::Translate(-position), e.GetPosition());
      switch(inputType)
      {
        case InputType::XAxis:
        case InputType::YAxis:
        case InputType::ZAxis: {
          Vec3f scalePos = Line{{0,0,0}, GetAxisVector(inputType)}.PointClosestFromLine(line);
          scale[(int)inputType] = (pressedScale / pressedScalePos * scalePos)[(int)inputType];
          return true;
        }
        case InputType::XPlane:
        case InputType::YPlane:
        case InputType::ZPlane: {
          Vec3f scalePos = Plane{GetAxisVector(inputType), {0,0,0}}.LineIntersection(line);
          scale = pressedScale / pressedScalePos.Length() * scalePos.Length();
          scale[(int)inputType - (int)InputType::XPlane] = pressedScale[(int)inputType - (int)InputType::XPlane];
          return true;
        }
        case InputType::FreeMove:
        case InputType::None:
          return false;
        default:
          Log::Warning("Input type not specified in switch statement");

      }
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_RELEASE))
    {
      if(inputType != InputType::None)
      {
        inputType = InputType::None;
        return true;
      }
    }

    return false;
  }


  void ScaleGizmo::RenderArrows(const Mat4& mirrorMatrix) const
  {
    axisMesh->Bind();

    shader->SetUniformColor4("uMaterialColor", Color(0.2,0.8,0.2));
    shader->SetUniformMat4("uTransformationMatrix", Mat4::Translate(position) * mirrorMatrix);
    axisMesh->Render();

    shader->SetUniformColor4("uMaterialColor", Color(0.2,0.2,0.8));
    shader->SetUniformMat4("uTransformationMatrix", Mat4::Translate(position) * mirrorMatrix * Mat4::RotateX(M_PI / 2));
    axisMesh->Render();

    shader->SetUniformColor4("uMaterialColor", Color(0.8,0.2,0.2));
    shader->SetUniformMat4("uTransformationMatrix", Mat4::Translate(position) * mirrorMatrix * Mat4::RotateZ(-M_PI / 2));
    axisMesh->Render();
  }

  void ScaleGizmo::RenderPlanes(const Mat4& mirrorMatrix) const
  {
    planeMesh->Bind();

    shader->SetUniformColor4("uMaterialColor", Color(0.2,0.8,0.2, 0.5));
    shader->SetUniformMat4("uTransformationMatrix", Mat4::Translate(position) * mirrorMatrix);
    planeMesh->Render();

    shader->SetUniformColor4("uMaterialColor", Color(0.2,0.2,0.8, 0.5));
    shader->SetUniformMat4("uTransformationMatrix", Mat4::Translate(position) * mirrorMatrix * Mat4::RotateX(-M_PI / 2));
    planeMesh->Render();

    shader->SetUniformColor4("uMaterialColor", Color(0.8,0.2,0.2, 0.5));
    shader->SetUniformMat4("uTransformationMatrix", Mat4::Translate(position) * mirrorMatrix * Mat4::RotateZ(M_PI / 2));
    planeMesh->Render();
  }

  void ScaleGizmo::RenderCube() const
  {
    cubeMesh->Bind();
    shader->SetUniformColor4("uMaterialColor", Color(0.8,0.8,0.8, 1.0));
    shader->SetUniformMat4("uTransformationMatrix", Mat4::Translate(position));
    cubeMesh->Render();
  }

  bool ScaleGizmo::IsInputTypeAxis(InputType type)
  {
    return type == InputType::XAxis || type == InputType::YAxis || type == InputType::ZAxis;
  }

  Vec3f ScaleGizmo::GetAxisVector(InputType type)
  {
    if(type == InputType::XAxis || type == InputType::XPlane)
      return {1, 0, 0};
    else if(type == InputType::YAxis || type == InputType::YPlane)
      return {0, 1, 0};
    else if(type == InputType::ZAxis || type == InputType::ZPlane)
      return {0, 0, 1};

    Log::Warning("GetAxisVector called with invalid InputType");
    return {0,0,0};
  }

  bool ScaleGizmo::IsInputTypePlane(InputType type)
  {
    return type == InputType::XPlane || type == InputType::YPlane || type == InputType::ZPlane;
  }
}
