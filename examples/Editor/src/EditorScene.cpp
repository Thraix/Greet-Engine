#include "EditorScene.h"

using namespace Greet;

EditorScene::EditorScene()
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
    Pointer<uint>{0, 1, 1, 3, 3, 2, 2, 0, 4, 5, 5, 7, 7, 6, 6, 4, 0, 4, 1, 5, 2, 6, 3, 7}
  );
  lineMesh->SetDrawMode(DrawType::LINES);
}

void EditorScene::Render3D() const
{
  ECSScene::Render3D();
  if(selectedEntity)
  {
    Entity camera{manager.get()};
    bool foundPrimary = false;
    manager->Each<Camera3DComponent>([&](EntityID id, Camera3DComponent& cam)
    {
      if(cam.active)
      {
        if(foundPrimary)
          Log::Warning("More than one primary 3D camera in scene");
        foundPrimary = true;
        camera.SetID(id);
      }
    });

    if(!foundPrimary)
    {
      Log::Warning("No camera in scene");
      return;
    }
    const BoundingBox& boundingBox = selectedEntity.GetComponent<MeshComponent>().mesh->GetBoundingBox();
    const Mat4& transform = selectedEntity.GetComponent<Transform3DComponent>().transform;
    glLineWidth(3);
    lineShader->Enable();
    Camera3DComponent& cameraComponent = camera.GetComponent<Camera3DComponent>();
    cameraComponent.SetShaderUniforms(lineShader);
    lineShader->SetUniformColor4("uMaterialColor", Color{0.8, 0.2, 0.8});
    lineShader->SetUniformMat4("uTransformationMatrix", transform * Mat4::Translate(boundingBox.GetPosition()- 0.01) * Mat4::Scale(boundingBox.GetSize() + 0.02));
    lineMesh->Bind();
    lineMesh->Render();
    lineMesh->Unbind();
    lineShader->Disable();
    glLineWidth(1);
    RenderCommand::ClearDepthBuffer();
    translateArrows.Render(cameraComponent, transform * boundingBox.GetCenter());
  }
}

void EditorScene::OnEvent(Event& event)
{
  ECSScene::OnEvent(event);
  if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
  {
    MousePressEvent e = static_cast<MousePressEvent&>(event);
    if(e.GetButton() != GREET_MOUSE_1)
      return;
    Entity camera{manager.get()};
    bool foundPrimary = false;
    manager->Each<Camera3DComponent>([&](EntityID id, Camera3DComponent& cam)
    {
      if(cam.active)
      {
        if(foundPrimary)
          Log::Warning("More than one primary 3D camera in scene");
        foundPrimary = true;
        camera.SetID(id);
      }
    });

    if(!foundPrimary)
    {
      Log::Warning("No camera in scene");
      return;
    }

    int i = 0;
    float distance = 100; // Limit
    Entity collisionEntity{manager.get()};
    Camera3DComponent& component = camera.GetComponent<Camera3DComponent>();
    manager->Each<Transform3DComponent, MeshComponent, MaterialComponent>([&](EntityID id, Transform3DComponent& transform, MeshComponent& mesh, MaterialComponent& material)
    {
      Line line = component.GetScreenToWorldCoordinate(Mat4::Inverse(transform.transform), e.GetPosition());
      std::pair<bool, float> collision = mesh.mesh->GetBoundingBox().LineIntersects(line, 100);
      if(collision.first && collision.second < distance)
      {
        collisionEntity.SetID(id);
        distance = collision.second;
      }
    });
    selectedEntity = collisionEntity;
    if(selectedEntity)
      Log::Info("selected entity");
  }

}
