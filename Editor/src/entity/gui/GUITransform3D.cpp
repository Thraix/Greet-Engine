#include "GUITransform3D.h"

#include "../EntityManager.h"
#include "GUIHelper.h"

#include <graphics/gui/Component.h>
#include <graphics/gui/Container.h>
#include <graphics/gui/TextBox.h>
#include <math/MathFunc.h>

namespace Greet
{
  GUITransform3D::GUITransform3D(EntityManager* entityManager, Container* parent)
    : entityManager{entityManager}, transform3D{ComponentFactory::GetComponent("res/guis/Transformation3DCompontent.xml", parent)}
  {
    transform3D->GetComponentByName<TextBox>("translationX")
      ->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxTranslation3DX));
    transform3D->GetComponentByName<TextBox>("translationY")
      ->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxTranslation3DY));
    transform3D->GetComponentByName<TextBox>("translationZ")
      ->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxTranslation3DZ));

    transform3D->GetComponentByName<TextBox>("scalingX")
      ->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxScaling3DX));
    transform3D->GetComponentByName<TextBox>("scalingY")
      ->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxScaling3DY));
    transform3D->GetComponentByName<TextBox>("scalingZ")
      ->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxScaling3DZ));

    transform3D->GetComponentByName<TextBox>("rotationX")
      ->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxRotation3DX));
    transform3D->GetComponentByName<TextBox>("rotationY")
      ->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxRotation3DY));
    transform3D->GetComponentByName<TextBox>("rotationZ")
      ->SetOnTextChangedCallback(BIND_MEMBER_FUNC(GUITextBoxRotation3DZ));
  }

  GUITransform3D::~GUITransform3D()
  {
    delete transform3D;
  }

  void GUITransform3D::Update(const Transform3DComponent& transform)
  {
    // Translation
    transform3D->GetComponentByName<TextBox>("translationX")
      ->SetText(GUIHelper::FloatToString(transform.GetPosition().x));
    transform3D->GetComponentByName<TextBox>("translationY")
      ->SetText(GUIHelper::FloatToString(transform.GetPosition().y));
    transform3D->GetComponentByName<TextBox>("translationZ")
      ->SetText(GUIHelper::FloatToString(transform.GetPosition().z));

    // Scale
    transform3D->GetComponentByName<TextBox>("scalingX")
      ->SetText(GUIHelper::FloatToString(transform.GetScale().x));
    transform3D->GetComponentByName<TextBox>("scalingY")
      ->SetText(GUIHelper::FloatToString(transform.GetScale().y));
    transform3D->GetComponentByName<TextBox>("scalingZ")
      ->SetText(GUIHelper::FloatToString(transform.GetScale().z));

    // Rotation
    transform3D->GetComponentByName<TextBox>("rotationX")
      ->SetText(GUIHelper::FloatToString(Math::ToDegrees(transform.GetRotation().x)));
    transform3D->GetComponentByName<TextBox>("rotationY")
      ->SetText(GUIHelper::FloatToString(Math::ToDegrees(transform.GetRotation().y)));
    transform3D->GetComponentByName<TextBox>("rotationZ")
      ->SetText(GUIHelper::FloatToString(Math::ToDegrees(transform.GetRotation().z)));
  }

  void GUITransform3D::AttachTo(Container* container, Transform3DComponent& transform)
  {
    container->AddComponent(transform3D);
    Update(transform);
  }

  void GUITransform3D::GUITextBoxTranslation3DX(Component* component,
                                                const std::string& before,
                                                const std::string& after)
  {
    if(!entityManager->GetSelectedEntity().HasComponent<Transform3DComponent>())
      return;

    Transform3DComponent& transform = entityManager->GetSelectedEntity().GetComponent<Transform3DComponent>();
    Vec3 pos = transform.GetPosition();
    pos.x = GUIHelper::StringToFloat(after);
    transform.SetPosition(pos);

    entityManager->UpdateSelectedTransform3D(NotifyOrigin::GUI);
  }

  void GUITransform3D::GUITextBoxTranslation3DY(Component* component,
                                                const std::string& before,
                                                const std::string& after)
  {
    if(!entityManager->GetSelectedEntity().HasComponent<Transform3DComponent>())
      return;

    Transform3DComponent& transform = entityManager->GetSelectedEntity().GetComponent<Transform3DComponent>();
    Vec3 pos = transform.GetPosition();
    pos.y = GUIHelper::StringToFloat(after);
    transform.SetPosition(pos);

    entityManager->UpdateSelectedTransform3D(NotifyOrigin::GUI);
  }

  void GUITransform3D::GUITextBoxTranslation3DZ(Component* component,
                                                const std::string& before,
                                                const std::string& after)
  {
    if(!entityManager->GetSelectedEntity().HasComponent<Transform3DComponent>())
      return;

    Transform3DComponent& transform = entityManager->GetSelectedEntity().GetComponent<Transform3DComponent>();
    Vec3 pos = transform.GetPosition();
    pos.z = GUIHelper::StringToFloat(after);
    transform.SetPosition(pos);

    entityManager->UpdateSelectedTransform3D(NotifyOrigin::GUI);
  }

  void GUITransform3D::GUITextBoxScaling3DX(Component* component,
                                            const std::string& before,
                                            const std::string& after)
  {
    if(!entityManager->GetSelectedEntity().HasComponent<Transform3DComponent>())
      return;

    Transform3DComponent& transform = entityManager->GetSelectedEntity().GetComponent<Transform3DComponent>();
    Vec3 scale = transform.GetScale();
    scale.x = GUIHelper::StringToFloat(after);
    transform.SetScale(scale);

    entityManager->UpdateSelectedTransform3D(NotifyOrigin::GUI);
  }

  void GUITransform3D::GUITextBoxScaling3DY(Component* component,
                                            const std::string& before,
                                            const std::string& after)
  {
    if(!entityManager->GetSelectedEntity().HasComponent<Transform3DComponent>())
      return;

    Transform3DComponent& transform = entityManager->GetSelectedEntity().GetComponent<Transform3DComponent>();
    Vec3 scale = transform.GetScale();
    scale.y = GUIHelper::StringToFloat(after);
    transform.SetScale(scale);

    entityManager->UpdateSelectedTransform3D(NotifyOrigin::GUI);
  }

  void GUITransform3D::GUITextBoxScaling3DZ(Component* component,
                                            const std::string& before,
                                            const std::string& after)
  {
    if(!entityManager->GetSelectedEntity().HasComponent<Transform3DComponent>())
      return;

    Transform3DComponent& transform = entityManager->GetSelectedEntity().GetComponent<Transform3DComponent>();
    Vec3 scale = transform.GetScale();
    scale.z = GUIHelper::StringToFloat(after);
    transform.SetScale(scale);

    entityManager->UpdateSelectedTransform3D(NotifyOrigin::GUI);
  }

  void GUITransform3D::GUITextBoxRotation3DX(Component* component,
                                             const std::string& before,
                                             const std::string& after)
  {
    if(!entityManager->GetSelectedEntity().HasComponent<Transform3DComponent>())
      return;

    Transform3DComponent& transform = entityManager->GetSelectedEntity().GetComponent<Transform3DComponent>();
    Vec3 rotation = transform.GetRotation();
    rotation.x = Math::ToRadians(GUIHelper::StringToFloat(after));
    transform.SetRotation(rotation);

    entityManager->UpdateSelectedTransform3D(NotifyOrigin::GUI);
  }

  void GUITransform3D::GUITextBoxRotation3DY(Component* component,
                                             const std::string& before,
                                             const std::string& after)
  {
    if(!entityManager->GetSelectedEntity().HasComponent<Transform3DComponent>())
      return;

    Transform3DComponent& transform = entityManager->GetSelectedEntity().GetComponent<Transform3DComponent>();
    Vec3 rotation = transform.GetRotation();
    rotation.y = Math::ToRadians(GUIHelper::StringToFloat(after));
    transform.SetRotation(rotation);

    entityManager->UpdateSelectedTransform3D(NotifyOrigin::GUI);
  }

  void GUITransform3D::GUITextBoxRotation3DZ(Component* component,
                                             const std::string& before,
                                             const std::string& after)
  {
    if(!entityManager->GetSelectedEntity().HasComponent<Transform3DComponent>())
      return;

    Transform3DComponent& transform = entityManager->GetSelectedEntity().GetComponent<Transform3DComponent>();
    Vec3 rotation = transform.GetRotation();
    rotation.z = Math::ToRadians(GUIHelper::StringToFloat(after));
    transform.SetRotation(rotation);

    entityManager->UpdateSelectedTransform3D(NotifyOrigin::GUI);
  }
}
