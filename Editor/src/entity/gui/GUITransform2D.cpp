#include "GUITransform2D.h"

#include "../EntityManager.h"
#include "GUIHelper.h"

#include <graphics/gui/Component.h>
#include <graphics/gui/Container.h>
#include <graphics/gui/TextBox.h>
#include <math/MathFunc.h>

namespace Greet
{
  GUITransform2D::GUITransform2D(EntityManager* entityManager, Container* parent)
    : entityManager{entityManager}, transform2D{ComponentFactory::GetComponent("res/guis/Transformation2DCompontent.xml", parent)}
  {
    transform2D->GetComponentByName<TextBox>("translationX")
      ->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxTranslationX));
    transform2D->GetComponentByName<TextBox>("translationY")
      ->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxTranslationY));

    transform2D->GetComponentByName<TextBox>("scalingX")
      ->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxScalingX));
    transform2D->GetComponentByName<TextBox>("scalingY")
      ->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxScalingY));

    transform2D->GetComponentByName<TextBox>("rotation")
      ->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxRotation));
  }

  GUITransform2D::~GUITransform2D()
  {
    delete transform2D;
  }

  void GUITransform2D::Update(const Transform2DComponent& transform)
  {
    // Translation
    transform2D->GetComponentByName<TextBox>("translationX")
      ->SetText(GUIHelper::FloatToString(transform.position.x));
    transform2D->GetComponentByName<TextBox>("translationY")
      ->SetText(GUIHelper::FloatToString(transform.position.y));

    // Scale
    transform2D->GetComponentByName<TextBox>("scalingX")
      ->SetText(GUIHelper::FloatToString(transform.scale.x));
    transform2D->GetComponentByName<TextBox>("scalingY")
      ->SetText(GUIHelper::FloatToString(transform.scale.y));

    // Rotation
    transform2D->GetComponentByName<TextBox>("rotation")
      ->SetText(GUIHelper::FloatToString(Math::ToDegrees(transform.rotation)));
  }

  void GUITransform2D::AttachTo(Container* container, Transform2DComponent& transform)
  {
    container->AddComponent(transform2D);
    Update(transform);
  }

  void GUITransform2D::GUITextBoxTranslationX(Component* component,
                                                const std::string& before,
                                                const std::string& after)
  {
    Entity e = entityManager->GetSelectedEntity();
    if(!e)
      return;
    ASSERT(e.HasComponent<Transform2DComponent>(), "Entity does not contain Transform2DComponent");

    e.GetComponent<Transform2DComponent>().position.x = GUIHelper::StringToFloat(after);

    entityManager->UpdateSelectedTransform2D(NotifyOrigin::GUI);
  }

  void GUITransform2D::GUITextBoxTranslationY(Component* component,
                                                const std::string& before,
                                                const std::string& after)
  {
    Entity e = entityManager->GetSelectedEntity();
    if(!e)
      return;
    ASSERT(e.HasComponent<Transform2DComponent>(), "Entity does not contain Transform2DComponent");

    e.GetComponent<Transform2DComponent>().position.y = GUIHelper::StringToFloat(after);

    entityManager->UpdateSelectedTransform2D(NotifyOrigin::GUI);
  }

  void GUITransform2D::GUITextBoxScalingX(Component* component,
                                            const std::string& before,
                                            const std::string& after)
  {
    Entity e = entityManager->GetSelectedEntity();
    if(!e)
      return;
    ASSERT(e.HasComponent<Transform2DComponent>(), "Entity does not contain Transform2DComponent");

    e.GetComponent<Transform2DComponent>().scale.x = GUIHelper::StringToFloat(after);

    entityManager->UpdateSelectedTransform2D(NotifyOrigin::GUI);
  }

  void GUITransform2D::GUITextBoxScalingY(Component* component,
                                            const std::string& before,
                                            const std::string& after)
  {
    Entity e = entityManager->GetSelectedEntity();
    if(!e)
      return;
    ASSERT(e.HasComponent<Transform2DComponent>(), "Entity does not contain Transform2DComponent");

    e.GetComponent<Transform2DComponent>().scale.y = GUIHelper::StringToFloat(after);

    entityManager->UpdateSelectedTransform2D(NotifyOrigin::GUI);
  }

  void GUITransform2D::GUITextBoxRotation(Component* component,
                                          const std::string& before,
                                          const std::string& after)
  {
    Entity e = entityManager->GetSelectedEntity();
    if(!e)
      return;
    ASSERT(e.HasComponent<Transform2DComponent>(), "Entity does not contain Transform2DComponent");

    e.GetComponent<Transform2DComponent>().rotation = Math::ToRadians(GUIHelper::StringToFloat(after));

    entityManager->UpdateSelectedTransform2D(NotifyOrigin::GUI);
  }
}
