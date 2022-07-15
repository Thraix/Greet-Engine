#pragma once

#include <graphics/gui/Container.h>
#include <ecs/components/Transform2DComponent.h>
#include <functional>

class EntityManager;

class GUITransform2D
{
  EntityManager* entityManager;

  Greet::Component* transform2D;

  DELETE_COPY_AND_MOVE(GUITransform2D);

  public:
    GUITransform2D(EntityManager* entityManager, Greet::Container* parent);
    virtual ~GUITransform2D();

    void Update(const Greet::Transform2DComponent& transform);
    void AttachTo(Greet::Container* container, Greet::Transform2DComponent& transform);

  private:
    void GUITextBoxTranslationX(Greet::Component* component,
                                const std::string& before,
                                const std::string& after);
    void GUITextBoxTranslationY(Greet::Component* component,
                                const std::string& before,
                                const std::string& after);
    void GUITextBoxScalingX(Greet::Component* component,
                            const std::string& before,
                            const std::string& after);
    void GUITextBoxScalingY(Greet::Component* component,
                            const std::string& before,
                            const std::string& after);
    void GUITextBoxRotationX(Greet::Component* component,
                             const std::string& before,
                             const std::string& after);
    void GUITextBoxRotationY(Greet::Component* component,
                             const std::string& before,
                             const std::string& after);
    void GUITextBoxRotation(Greet::Component* component,
                             const std::string& before,
                             const std::string& after);
};
