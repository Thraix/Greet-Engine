#include "ColorPicker.h"

#include <functional>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(ColorPicker);

  ColorPicker::ColorPicker(const XMLObject& object, Component* parent)
    : Component{object, parent}, attachedColor{nullptr}
  {
    picker = new ColorPickerWindow(Vec2f(0,0), GUIUtils::GetColorFromXML(object, "color", Color{1,1,1,1}));
    picker->SetOnColorChangeCallback(BIND_MEMBER_FUNC(OnColorChanged));
  }

  ColorPicker::~ColorPicker()
  {
    delete picker;
  }

  void ColorPicker::PreRender(GUIRenderer* renderer, const Vec2f& translation) const
  {
    renderer->PushTranslation(translation);

    // Border around Component
    if(borderColor.a != 0.0)
      //renderer->SubmitRect(pos + Vec2f(0,0), size, currentStyle->borderColor, false);
      renderer->DrawRoundedRect(pos+Vec2f(0,0), {width.size, height.size}, borderColor, borderRadius, borderRoundedPrecision, false);

    // Component background
    renderer->DrawRoundedRect(pos + border.LeftTop(), Vec2f{width.size, height.size} - GetBorder().LeftTop()-GetBorder().RightBottom(), GetColor(), backgroundRadius, backgroundRoundedPrecision, false);
  }

  void ColorPicker::AttachColor(Color* color)
  {
    attachedColor = color;
  }

  void ColorPicker::OnEvent(Event& event, const Vec2f& componentPos)
  {
    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      guiScene->AddFrameQueued(picker);
      picker->SetPosition(e.GetPosition());
      guiScene->RequestFocusQueued(picker);
    }
  }

  void ColorPicker::OnColorChanged(const Color& previous, const Color& current)
  {
    if(attachedColor)
      *attachedColor = current;
    CallOnColorChangeCallback(previous,current);
  }

  void ColorPicker::SetOnColorChangeCallback(OnColorChangeCallback callback)
  {
    onColorChangeCallback = callback;
    CallOnColorChangeCallback({0, 0, 0}, GetColor());
  }

  void ColorPicker::CallOnColorChangeCallback(const Color& previous, const Color& current)
  {
    if(onColorChangeCallback)
      onColorChangeCallback(this, previous, current);
  }

  const Color& ColorPicker::GetColor() const
  {
    return picker->GetColor();
  }
}
