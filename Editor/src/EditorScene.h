#include <Greet.h>

#include "graphics/TranslateArrowsRenderer.h"

class EditorScene : public Greet::ECSScene
{
  TranslateArrowsRenderer translateArrows;
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
