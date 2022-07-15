#pragma once

#include <graphics/Color.h>
#include <graphics/gui/Frame.h>
#include <graphics/gui/SatValSlider.h>
#include <graphics/gui/HueSlider.h>
#include <graphics/gui/TextBox.h>

namespace Greet
{
  class ColorPickerWindow : public Frame
  {
    protected:
      enum class InputChangeType
      {
        NONE, SLIDER, RGB_TEXTBOX, HSV_TEXTBOX, HEX_TEXTBOX
      };
    public:
      typedef std::function<void(const Color& previous, const Color& current)> OnColorChangeCallback;

    protected:
      static XMLObject frameLook;
      static MetaFile frameStyle;
      Color color;
      HueSlider* hSlider;
      SatValSlider* svSlider;

      TextBox* rTextBox;
      TextBox* gTextBox;
      TextBox* bTextBox;
      TextBox* hTextBox;
      TextBox* sTextBox;
      TextBox* vTextBox;
      TextBox* hexTextBox;

      Component* colorDisplay;

      OnColorChangeCallback onColorChangeCallback;

    protected:
      void SliderChanged(Component* component, float oldValue, float newValue);
      void RGBTextBoxChanged(Component* textBox, const std::string& oldText, const std::string& newText);
      void HSVTextBoxChanged(Component* textBox, const std::string& oldText, const std::string& newText);
      void HexTextBoxChanged(Component* textBox, const std::string& oldText, const std::string& newText);
      void UpdateColor(const Color& avColor, InputChangeType type, bool rgb, bool callback);

    public:
      ColorPickerWindow();
      ColorPickerWindow(const Vec2f& pos, const Color& color);

      void SetOnColorChangeCallback(OnColorChangeCallback callback);
      void CallOnColorChangeCallback(const Color& previous, const Color& current);

      void SetColor(const Color&);
      const Color& GetColor() const;
  };
}
