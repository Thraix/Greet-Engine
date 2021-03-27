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
  Entity camera = GetCameraEntity();
  DrawBoundingBoxes(camera.GetComponent<Camera3DComponent>());
  if(selectedEntity)
  {
    RenderCommand::ClearDepthBuffer();
    Camera3DComponent& cameraComponent = camera.GetComponent<Camera3DComponent>();
    translationGizmo.Render(cameraComponent);
  }
}

void EditorScene::DrawBoundingBoxes(const Camera3DComponent& cameraComponent) const
{
    glLineWidth(2);
  lineShader->Enable();
  cameraComponent.SetShaderUniforms(lineShader);
  lineMesh->Bind();
  manager->Each<Transform3DComponent, MaterialComponent, MeshComponent>([&](EntityID id, const Transform3DComponent& transform, const MaterialComponent&, const MeshComponent& mesh)
  {
    if(id == selectedEntity.GetID())
      lineShader->SetUniformColor4("uMaterialColor", Color{0.8, 0.2, 0.8});
    else
      lineShader->SetUniformColor4("uMaterialColor", Color{0.2, 0.2, 0.2});
    const BoundingBox& boundingBox = mesh.mesh->GetBoundingBox();
    lineShader->SetUniformMat4("uTransformationMatrix", transform.transform *  Mat4::Scale(boundingBox.GetSize()) * Mat4::Translate(-0.5));
    lineMesh->Render();
  });
  lineMesh->Unbind();
  lineShader->Disable();
    glLineWidth(1);
}

void EditorScene::OnEvent(Event& event)
{
  ECSScene::OnEvent(event);
  Entity camera = GetCameraEntity();

  if(!camera)
  {
    Log::Warning("No camera in scene");
    return;
  }
  Camera3DComponent& component = camera.GetComponent<Camera3DComponent>();

  if(selectedEntity)
  {
    Transform3DComponent& transform = selectedEntity.GetComponent<Transform3DComponent>();
    translationGizmo.position = transform.GetPosition();
    translationGizmo.scale = transform.GetScale();
    bool handled = translationGizmo.OnEvent(event, component);
    if(handled)
    {
      transform.SetPosition(translationGizmo.position);
      transform.SetScale(translationGizmo.scale);
      return;
    }
  }

  if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
  {

    MousePressEvent e = static_cast<MousePressEvent&>(event);
    if(e.GetButton() != GREET_MOUSE_1)
      return;

    int i = 0;
    float distance = 100; // Limit
    Entity collisionEntity{manager.get()};
    manager->Each<Transform3DComponent, MeshComponent, MaterialComponent>([&](EntityID id, Transform3DComponent& transform, MeshComponent& mesh, MaterialComponent& material)
    {
      Line line = component.GetScreenToWorldCoordinate(e.GetPosition());
      std::pair<bool, float> collision = mesh.mesh->GetBoundingBox().LineIntersects(transform.transform, line);
      if(collision.first && collision.second < distance)
      {
        collisionEntity.SetID(id);
        distance = collision.second;
      }
      i++;
    });
    selectedEntity = collisionEntity;
    if(selectedEntity)
    {
      Transform3DComponent& transform = selectedEntity.GetComponent<Transform3DComponent>();
      translationGizmo.position = transform.GetPosition();
      translationGizmo.scale = transform.GetScale();
    }
  }
}

Entity EditorScene::GetCameraEntity() const
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
    return {};
  }
  return camera;
}
