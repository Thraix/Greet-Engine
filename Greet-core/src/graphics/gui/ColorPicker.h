#pragma once

#include <graphics/gui/Component.h>
#include <graphics/gui/ColorPickerWindow.h>

namespace Greet
{
  class ColorPicker : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(ColorPicker);
    public:
      typedef std::function<void(Component* comp, const Color& previous, const Color& current)> OnColorChangeCallback;
    public:
      OnColorChangeCallback onColorChangeCallback;
    private:
      Color* attachedColor;
      ColorPickerWindow* picker;
    public:
      ColorPicker(const XMLObject& object, Component* parent);
      virtual ~ColorPicker();

      void PreRender(GUIRenderer* renderer, const Vec2f& translation) const override;

      void AttachColor(Color* color);

      void OnEvent(Event& event, const Vec2f& translatedPos) override;
      void OnColorChanged(const Color& previous, const Color& current);

      void SetOnColorChangeCallback(OnColorChangeCallback callback);
      void CallOnColorChangeCallback(const Color& previous, const Color& current);

      void SetColor(const Color& color);
      const Color& GetColor() const;
  };

}
