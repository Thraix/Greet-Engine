#include "EntityRendering.h"

using namespace Greet;

EntityRendering::EntityRendering(ECSManager* manager)
  : manager{manager}
{
  lineShader = Shader::FromFile("res/shaders/simple.shader");
  lineMesh = NewRef<Mesh>(
    Pointer<Vec3f>{
      {1,1,1},
      {0,1,1},
      {1,0,1},
      {0,0,1},
      {1,1,0},
      {0,1,0},
      {1,0,0},
      {0,0,0},
    },
    Pointer<uint32_t>{0, 1, 1, 3, 3, 2, 2, 0, 4, 5, 5, 7, 7, 6, 6, 4, 0, 4, 1, 5, 2, 6, 3, 7}
  );
  lineMesh->SetDrawMode(DrawType::LINES);
}

void EntityRendering::RenderOutlines(const Camera3DComponent& cameraComponent, Entity selectedEntity)
{
  glLineWidth(3);
  lineShader->Enable();
  cameraComponent.SetShaderUniforms(lineShader);
  lineMesh->Bind();
  manager->Each<Transform3DComponent, MaterialComponent, MeshComponent>(
    [&](EntityID id, const Transform3DComponent& transform, const MaterialComponent&, const MeshComponent& mesh)
    {
      if(id == selectedEntity.GetID())
        lineShader->SetUniformColor4("uMaterialColor", Color{0.8, 0.2, 0.8});
      else
        lineShader->SetUniformColor4("uMaterialColor", Color{0.2, 0.2, 0.2});
      const BoundingBox& boundingBox = mesh.mesh->GetBoundingBox();
      lineShader->SetUniformMat4("uTransformationMatrix", transform.transform *  Mat4::Scale(boundingBox.GetSize() + 0.01) * Mat4::Translate(-0.5));
      lineMesh->Render();
    });
  lineMesh->Unbind();
  lineShader->Disable();
  glLineWidth(1);
}

void EntityRendering::RenderGizmo(const Camera3DComponent& cameraComponent, Entity selectedEntity)
{
  if(selectedEntity)
  {
    RenderCommand::EnableDepthTest(false);
    scaleGizmo.Render(cameraComponent);
    RenderCommand::ResetDepthTest();
  }
}

TransformGizmo& EntityRendering::GetTransformGizmo()
{
  return scaleGizmo;
}
