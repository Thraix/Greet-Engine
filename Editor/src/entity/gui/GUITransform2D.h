#pragma once

#include <common/Memory.h>
#include <ecs/components/Transform2DComponent.h>

namespace Greet
{
  class Component;
  class Container;
  class EntityManager;

  class GUITransform2D
  {
    DELETE_COPY_AND_MOVE(GUITransform2D);

    EntityManager* entityManager;
    Component* transform2D;

    public:
      GUITransform2D(EntityManager* entityManager, Container* parent);
      virtual ~GUITransform2D();

      void Update(const Transform2DComponent& transform);
      void AttachTo(Container* container, Transform2DComponent& transform);

    private:
      void GUITextBoxTranslationX(Component* component,
                                  const std::string& before,
                                  const std::string& after);
      void GUITextBoxTranslationY(Component* component,
                                  const std::string& before,
                                  const std::string& after);
      void GUITextBoxScalingX(Component* component,
                              const std::string& before,
                              const std::string& after);
      void GUITextBoxScalingY(Component* component,
                              const std::string& before,
                              const std::string& after);
      void GUITextBoxRotationX(Component* component,
                               const std::string& before,
                               const std::string& after);
      void GUITextBoxRotationY(Component* component,
                               const std::string& before,
                               const std::string& after);
      void GUITextBoxRotation(Component* component,
                               const std::string& before,
                               const std::string& after);
  };
}
