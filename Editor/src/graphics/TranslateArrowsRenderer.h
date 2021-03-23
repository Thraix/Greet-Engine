#pragma once

#include <Greet.h>

enum class InputType
{
  XAxis = 0, YAxis = 1, ZAxis = 2, XPlane= 3, YPlane = 4, ZPlane = 5, FreeMove = 6, None
};

class TranslateArrowsRenderer
{
  public:
    Greet::Vec3f position;
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

    TranslateArrowsRenderer()
    {
      axisMesh = Greet::NewRef<Greet::Mesh>(Greet::OBJUtils::LoadObj("res/objs/axis.obj"));
      planeMesh = Greet::NewRef<Greet::Mesh>(Greet::MeshFactory::Plane({0.5,0,0.5}, {0.4}));
      cubeMesh = Greet::NewRef<Greet::Mesh>(Greet::MeshFactory::Cube({0,0,0}, {0.2}));
      planeMesh->SetEnableCulling(false);
      shader = Greet::Shader::FromFile("res/shaders/directional_light.shader");
      Greet::Log::Info(axisMesh->GetBoundingBox());

      // Axis bounding boxes
      Greet::BoundingBox boxYAxis = axisMesh->GetBoundingBox();
      Greet::BoundingBox boxXAxis = axisMesh->GetBoundingBox();
      std::swap(boxXAxis.boundingBoxMax.x, boxXAxis.boundingBoxMax.y);
      std::swap(boxXAxis.boundingBoxMin.x, boxXAxis.boundingBoxMin.y);
      Greet::BoundingBox boxZAxis = axisMesh->GetBoundingBox();
      std::swap(boxZAxis.boundingBoxMax.z, boxZAxis.boundingBoxMax.y);
      std::swap(boxZAxis.boundingBoxMin.z, boxZAxis.boundingBoxMin.y);

      boundingBoxes.emplace_back(boxXAxis, InputType::XAxis);
      boundingBoxes.emplace_back(boxYAxis, InputType::YAxis);
      boundingBoxes.emplace_back(boxZAxis, InputType::ZAxis);

      // Plane bounding boxes
      Greet::BoundingBox boxYPlane = planeMesh->GetBoundingBox();
      Greet::BoundingBox boxXPlane = planeMesh->GetBoundingBox();
      std::swap(boxXPlane.boundingBoxMax.x, boxXPlane.boundingBoxMax.y);
      std::swap(boxXPlane.boundingBoxMin.x, boxXPlane.boundingBoxMin.y);
      Greet::BoundingBox boxZPlane = planeMesh->GetBoundingBox();
      std::swap(boxZPlane.boundingBoxMax.z, boxZPlane.boundingBoxMax.y);
      std::swap(boxZPlane.boundingBoxMin.z, boxZPlane.boundingBoxMin.y);

      boundingBoxes.emplace_back(boxXPlane, InputType::XPlane);
      boundingBoxes.emplace_back(boxYPlane, InputType::YPlane);
      boundingBoxes.emplace_back(boxZPlane, InputType::ZPlane);

      // Free move bounding box
      boundingBoxes.emplace_back(cubeMesh->GetBoundingBox(), InputType::FreeMove);
    }

    void Render(const Greet::Camera3DComponent& camera) const
    {
      shader->Enable();
      camera.SetShaderUniforms(shader);
      shader->SetUniform1f("uSpecularStrength", 0.0f);
      shader->SetUniform1f("uAmbient", 0.7f);
      shader->SetUniformBoolean("uHasTexture", false);
      RenderCube();
      RenderArrows();
      RenderPlanes();
    }

    void RenderArrows() const
    {
      axisMesh->Bind();

      shader->SetUniformColor4("uMaterialColor", Greet::Color(0.2,0.8,0.2));
      shader->SetUniformMat4("uTransformationMatrix", Greet::Mat4::Translate(position));
      axisMesh->Render();

      shader->SetUniformColor4("uMaterialColor", Greet::Color(0.2,0.2,0.8));
      shader->SetUniformMat4("uTransformationMatrix", Greet::Mat4::Translate(position) * Greet::Mat4::RotateX(M_PI / 2));
      axisMesh->Render();

      shader->SetUniformColor4("uMaterialColor", Greet::Color(0.8,0.2,0.2));
      shader->SetUniformMat4("uTransformationMatrix", Greet::Mat4::Translate(position) * Greet::Mat4::RotateZ(-M_PI / 2));
      axisMesh->Render();
    }

    void RenderPlanes() const
    {
      planeMesh->Bind();

      shader->SetUniformColor4("uMaterialColor", Greet::Color(0.2,0.8,0.2, 0.5));
      shader->SetUniformMat4("uTransformationMatrix", Greet::Mat4::Translate(position));
      planeMesh->Render();

      shader->SetUniformColor4("uMaterialColor", Greet::Color(0.2,0.2,0.8, 0.5));
      shader->SetUniformMat4("uTransformationMatrix", Greet::Mat4::Translate(position) * Greet::Mat4::RotateX(-M_PI / 2));
      planeMesh->Render();

      shader->SetUniformColor4("uMaterialColor", Greet::Color(0.8,0.2,0.2, 0.5));
      shader->SetUniformMat4("uTransformationMatrix", Greet::Mat4::Translate(position) * Greet::Mat4::RotateZ(M_PI / 2));
      planeMesh->Render();
    }

    void RenderCube() const
    {
      cubeMesh->Bind();
      shader->SetUniformColor4("uMaterialColor", Greet::Color(0.8,0.8,0.8, 1.0));
      shader->SetUniformMat4("uTransformationMatrix", Greet::Mat4::Translate(position));
      cubeMesh->Render();
    }

    bool OnEvent(Greet::Event& event, const Greet::Camera3DComponent& cameraComponent)
    {
      if(EVENT_IS_TYPE(event, Greet::EventType::MOUSE_PRESS))
      {
        Greet::MousePressEvent e = static_cast<Greet::MousePressEvent&>(event);
        if(e.GetButton() != GREET_MOUSE_1)
          return false;

        Greet::Line line = cameraComponent.GetScreenToWorldCoordinate(Greet::Mat4::Translate(-position), e.GetPosition());
        int minDistance = 100;
        inputType = InputType::None;
        for(auto&& boundingBox : boundingBoxes)
        {
          std::pair<bool, float> collision = boundingBox.first.LineIntersects(line, minDistance);
          if(collision.first)
          {
            minDistance = collision.second;
            inputType = boundingBox.second;
            pressedEntityPos = position;
          }
        }
        if(IsInputTypeAxis(inputType))
          pressedTranslationPos = Greet::Line{{0,0,0}, GetAxisVector(inputType)}.PointClosestFromLine(line);
        else if(IsInputTypePlane(inputType))
          pressedTranslationPos = Greet::Plane{GetAxisVector(inputType), {0,0,0}}.LineIntersection(line);
        Greet::Log::Info(pressedTranslationPos);
      }
      else if(EVENT_IS_TYPE(event, Greet::EventType::MOUSE_MOVE))
      {
        Greet::MouseMoveEvent e = static_cast<Greet::MouseMoveEvent&>(event);
        Greet::Line line = cameraComponent.GetScreenToWorldCoordinate(Greet::Mat4::Translate(-pressedEntityPos), e.GetPosition());
        switch(inputType)
        {
          case InputType::XAxis:
          case InputType::YAxis:
          case InputType::ZAxis: {
            Greet::Vec3f translationPos = Greet::Line{{0,0,0}, GetAxisVector(inputType)}.PointClosestFromLine(line);
            position = pressedEntityPos + (translationPos - pressedTranslationPos);
            return true;
          }
          case InputType::XPlane:
          case InputType::YPlane:
          case InputType::ZPlane: {
            Greet::Vec3f translationPos = Greet::Plane{GetAxisVector(inputType), {0,0,0}}.LineIntersection(line);
            position = pressedEntityPos + (translationPos - pressedTranslationPos);
            return true;
          }
          case InputType::FreeMove:
          case InputType::None:
            break;
          default:
            Greet::Log::Warning("Input type not specified in switch statement");

        }
      }
      else if(EVENT_IS_TYPE(event, Greet::EventType::MOUSE_RELEASE))
      {
        if(inputType != InputType::None)
        {
          inputType = InputType::None;
          return true;
        }
      }

      return false;
    }


    bool IsInputTypeAxis(InputType type)
    {
      return type == InputType::XAxis || type == InputType::YAxis || type == InputType::ZAxis;
    }

    Greet::Vec3f GetAxisVector(InputType type)
    {
      if(type == InputType::XAxis || type == InputType::XPlane)
        return {1, 0, 0};
      else if(type == InputType::YAxis || type == InputType::YPlane)
        return {0, 1, 0};
      else if(type == InputType::ZAxis || type == InputType::ZPlane)
        return {0, 0, 1};

      Greet::Log::Warning("GetAxisVector called with invalid InputType");
      return {0,0,0};
    }

    bool IsInputTypePlane(InputType type)
    {
      return type == InputType::XPlane || type == InputType::YPlane || type == InputType::ZPlane;
    }
};
