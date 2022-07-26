#pragma once

#include <common/Memory.h>
#include <ecs/components/Transform3DComponent.h>

namespace Greet
{
  class Component;
  class Container;
  class EntityManager;

  class GUITransform3D
  {
    DELETE_COPY_AND_MOVE(GUITransform3D);

    EntityManager* entityManager;
    Component* transform3D;


    public:
      GUITransform3D(EntityManager* entityManager, Container* parent);
      virtual ~GUITransform3D();

      void Update(const Transform3DComponent& transform);
      void AttachTo(Container* container, Transform3DComponent& transform);

    private:
      void GUITextBoxTranslation3DX(Component* component,
                                    const std::string& before,
                                    const std::string& after);
      void GUITextBoxTranslation3DY(Component* component,
                                    const std::string& before,
                                    const std::string& after);
      void GUITextBoxTranslation3DZ(Component* component,
                                    const std::string& before,
                                    const std::string& after);
      void GUITextBoxScaling3DX(Component* component,
                                const std::string& before,
                                const std::string& after);
      void GUITextBoxScaling3DY(Component* component,
                                const std::string& before,
                                const std::string& after);
      void GUITextBoxScaling3DZ(Component* component,
                                const std::string& before,
                                const std::string& after);
      void GUITextBoxRotation3DX(Component* component,
                                 const std::string& before,
                                 const std::string& after);
      void GUITextBoxRotation3DY(Component* component,
                                 const std::string& before,
                                 const std::string& after);
      void GUITextBoxRotation3DZ(Component* component,
                                 const std::string& before,
                                 const std::string& after);
  };
}
