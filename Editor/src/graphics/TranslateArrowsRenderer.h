#pragma once

#include <Greet.h>

class TranslateArrowsRenderer
{
  private:
    Greet::Ref<Greet::Mesh> mesh;
    Greet::Ref<Greet::Shader> shader;
  public:

    TranslateArrowsRenderer()
    {
      mesh = Greet::NewRef<Greet::Mesh>(Greet::OBJUtils::LoadObj("res/objs/translate_arrow.obj"));
      shader = Greet::ShaderFactory::Shader3D();
      Greet::Log::Info(mesh->GetBoundingBox());
    }

    void Render(const Greet::Camera3DComponent& camera, const Greet::Vec3f& pos) const
    {
      shader->Enable();
      camera.SetShaderUniforms(shader);
      shader->SetUniformBoolean("uHasTexture", false);
      shader->SetUniformColor4("uMaterialColor", Greet::Color(0.2,0.8,0.2));
      shader->SetUniformMat4("uTransformationMatrix", Greet::Mat4::Translate(pos));
      mesh->Bind();
      mesh->Render();
      shader->SetUniformColor4("uMaterialColor", Greet::Color(0.2,0.2,0.8));
      shader->SetUniformMat4("uTransformationMatrix", Greet::Mat4::Translate(pos) * Greet::Mat4::RotateX(-M_PI / 2));
      mesh->Render();
      shader->SetUniformColor4("uMaterialColor", Greet::Color(0.8,0.2,0.2));
      shader->SetUniformMat4("uTransformationMatrix", Greet::Mat4::Translate(pos) * Greet::Mat4::RotateZ(-M_PI / 2));
      mesh->Render();
      mesh->Unbind();
      shader->Disable();
    }
};
