#pragma once

#include <Greet.h>

enum class SelectedArrow
{
  X = 0, Y = 1, Z = 2, None
};

class TranslateArrowsRenderer
{
  public:
    Greet::Vec3f position;
  private:
    Greet::Ref<Greet::Mesh> mesh;
    Greet::Ref<Greet::Shader> shader;

    SelectedArrow selectedArrow = SelectedArrow::None;
    Greet::Vec3f pressedEntityPos;
    Greet::Vec3f pressedTranslationPos;


  public:

    TranslateArrowsRenderer()
    {
      mesh = Greet::NewRef<Greet::Mesh>(Greet::OBJUtils::LoadObj("res/objs/translate_arrow.obj"));
      shader = Greet::ShaderFactory::Shader3D();
      Greet::Log::Info(mesh->GetBoundingBox());
    }

    void Render(const Greet::Camera3DComponent& camera) const
    {
      shader->Enable();
      camera.SetShaderUniforms(shader);
      shader->SetUniformBoolean("uHasTexture", false);
        shader->SetUniformColor4("uMaterialColor", Greet::Color(0.2,0.8,0.2));
      shader->SetUniformMat4("uTransformationMatrix", Greet::Mat4::Translate(position));
      mesh->Bind();
      mesh->Render();
        shader->SetUniformColor4("uMaterialColor", Greet::Color(0.2,0.2,0.8));
      shader->SetUniformMat4("uTransformationMatrix", Greet::Mat4::Translate(position) * Greet::Mat4::RotateX(M_PI / 2));
      mesh->Render();
        shader->SetUniformColor4("uMaterialColor", Greet::Color(0.8,0.2,0.2));
      shader->SetUniformMat4("uTransformationMatrix", Greet::Mat4::Translate(position) * Greet::Mat4::RotateZ(-M_PI / 2));
      mesh->Render();
      mesh->Unbind();
      shader->Disable();
    }

    bool OnEvent(Greet::Event& event, const Greet::Camera3DComponent& cameraComponent)
    {
      if(EVENT_IS_TYPE(event, Greet::EventType::MOUSE_PRESS))
      {
        Greet::MousePressEvent e = static_cast<Greet::MousePressEvent&>(event);
        if(e.GetButton() != GREET_MOUSE_1)
          return false;
        Greet::BoundingBox boxY = mesh->GetBoundingBox();
        Greet::BoundingBox boxX = mesh->GetBoundingBox();
        std::swap(boxX.boundingBoxMax.x, boxX.boundingBoxMax.y);
        std::swap(boxX.boundingBoxMin.x, boxX.boundingBoxMin.y);
        Greet::BoundingBox boxZ = mesh->GetBoundingBox();
        std::swap(boxZ.boundingBoxMax.z, boxZ.boundingBoxMax.y);
        std::swap(boxZ.boundingBoxMin.z, boxZ.boundingBoxMin.y);
        Greet::Line line = cameraComponent.GetScreenToWorldCoordinate(Greet::Mat4::Translate(-position), e.GetPosition());
        if(boxX.LineIntersects(line, 100).first)
        {
          selectedArrow = SelectedArrow::X;
          pressedEntityPos = position;
          pressedTranslationPos = Greet::Line{{0,0,0}, {1, 0, 0}}.PointClosestFromLine(line);
          return true;
        }
        else if(boxY.LineIntersects(line, 100).first)
        {
          selectedArrow = SelectedArrow::Y;
          pressedEntityPos = position;
          pressedTranslationPos = Greet::Line{{0,0,0}, {0, 1, 0}}.PointClosestFromLine(line);
          return true;
        }
        else if(boxZ.LineIntersects(line, 100).first)
        {
          selectedArrow = SelectedArrow::Z;
          pressedEntityPos = position;
          pressedTranslationPos = Greet::Line{{0,0,0}, {0, 0, 1}}.PointClosestFromLine(line);
          return true;
        }
        else
        {
          selectedArrow = SelectedArrow::None;
        }
      }
      else if(EVENT_IS_TYPE(event, Greet::EventType::MOUSE_MOVE))
      {
        if(selectedArrow == SelectedArrow::None)
          return false;
        Greet::MouseMoveEvent e = static_cast<Greet::MouseMoveEvent&>(event);
        Greet::Line line = cameraComponent.GetScreenToWorldCoordinate(Greet::Mat4::Translate(-pressedEntityPos), e.GetPosition());
        Greet::Vec3f arrowDir{0};
        arrowDir[(int)selectedArrow] = 1;
        Greet::Vec3f translationPos = Greet::Line{{0,0,0}, arrowDir}.PointClosestFromLine(line);
        position = pressedEntityPos + (translationPos - pressedTranslationPos);
        return true;
      }
      else if(EVENT_IS_TYPE(event, Greet::EventType::MOUSE_RELEASE))
      {
        if(selectedArrow != SelectedArrow::None)
        {
          selectedArrow = SelectedArrow::None;
          return true;
        }
      }

      return false;
    }
};
