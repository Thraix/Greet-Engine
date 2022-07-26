#pragma once

#include <ecs/Entity.h>
#include <common/Memory.h>
#include "../gizmos/TranslationGizmo.h"

namespace Greet
{
  class ECSManager;
  class Mesh;
  class Shader;
  class Camera3DComponent;

  class EntityManager;

  class EntityRendering
  {
    TranslationGizmo scaleGizmo;
    Ref<Shader> lineShader;
    Ref<Mesh> lineMesh;
    ECSManager* manager;

    public:
      EntityRendering(ECSManager* manager);
      void RenderOutlines(const Camera3DComponent& cameraComponent, Entity selectedEntity);
      void RenderGizmo(const Camera3DComponent& cameraComponent, Entity selectedEntity);

      TransformGizmo& GetTransformGizmo();
  };
}
