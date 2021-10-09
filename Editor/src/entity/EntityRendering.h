#pragma once

#include "../gizmos/ScaleGizmo.h"
#include "../gizmos/TranslationGizmo.h"

#include <graphics/shaders/Shader.h>
#include <graphics/models/Mesh.h>

class EntityRendering
{
  TranslationGizmo scaleGizmo;
  Greet::Ref<Greet::Shader> lineShader;
  Greet::Ref<Greet::Mesh> lineMesh;
  Greet::ECSManager* manager;

  public:
    EntityRendering(Greet::ECSManager* manager);
    void RenderOutlines(const Greet::Camera3DComponent& cameraComponent, Greet::Entity selectedEntity);
    void RenderGizmo(const Greet::Camera3DComponent& cameraComponent, Greet::Entity selectedEntity);

    TransformGizmo& GetTransformGizmo();
};
