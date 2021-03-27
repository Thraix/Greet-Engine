#include <Greet.h>

#include "gizmos/TranslationGizmo.h"
#include "gizmos/ScaleGizmo.h"

class EditorScene : public Greet::ECSScene
{
  ScaleGizmo translationGizmo;
  Greet::Ref<Greet::Shader> lineShader;
  Greet::Ref<Greet::Mesh> lineMesh;

  Greet::Entity selectedEntity;

  public:
    EditorScene();

    void OnEvent(Greet::Event& event) override;
    void Render3D() const override;
    void DrawBoundingBoxes(const Greet::Camera3DComponent& cameraComponent) const;

    Greet::Entity GetCameraEntity() const;
};
