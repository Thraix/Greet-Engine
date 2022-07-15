#pragma once

#include <graphics/gui/Container.h>
#include <ecs/components/Transform3DComponent.h>
#include <functional>

class EntityManager;

class GUITransform3D
{
  EntityManager* entityManager;

  Greet::Component* transform3D;

  DELETE_COPY_AND_MOVE(GUITransform3D);

  public:
    GUITransform3D(EntityManager* entityManager, Greet::Container* parent);
    virtual ~GUITransform3D();

    void Update(const Greet::Transform3DComponent& transform);
    void AttachTo(Greet::Container* container, Greet::Transform3DComponent& transform);

  private:
    void GUITextBoxTranslation3DX(Greet::Component* component,
                                  const std::string& before,
                                  const std::string& after);
    void GUITextBoxTranslation3DY(Greet::Component* component,
                                  const std::string& before,
                                  const std::string& after);
    void GUITextBoxTranslation3DZ(Greet::Component* component,
                                  const std::string& before,
                                  const std::string& after);
    void GUITextBoxScaling3DX(Greet::Component* component,
                              const std::string& before,
                              const std::string& after);
    void GUITextBoxScaling3DY(Greet::Component* component,
                              const std::string& before,
                              const std::string& after);
    void GUITextBoxScaling3DZ(Greet::Component* component,
                              const std::string& before,
                              const std::string& after);
    void GUITextBoxRotation3DX(Greet::Component* component,
                               const std::string& before,
                               const std::string& after);
    void GUITextBoxRotation3DY(Greet::Component* component,
                               const std::string& before,
                               const std::string& after);
    void GUITextBoxRotation3DZ(Greet::Component* component,
                               const std::string& before,
                               const std::string& after);
};
