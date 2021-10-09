#pragma once

#include "../gizmos/ScaleGizmo.h"
#include "../gizmos/TranslationGizmo.h"
#include <ecs/Entity.h>

namespace Greet
{
  class ECSManager;
  class Mesh;
  class Shader;
};

class EntityManager;

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
