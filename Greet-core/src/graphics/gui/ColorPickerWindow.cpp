#include "ColorPickerWindow.h"

#include <graphics/gui/Label.h>
#include <utils/LogUtils.h>

#include <functional>
#include <sstream>

namespace Greet
{
  XMLObject ColorPickerWindow::frameLook = XMLObject
  {
#include "ColorPickerLook.xml"
  };

  MetaFile ColorPickerWindow::frameStyle = MetaFile
  {
    std::istringstream(
#include "ColorPickerStyle.meta"
    )
  };

  ColorPickerWindow::ColorPickerWindow(const Vec2f& pos, const Color& color)
    : Frame(frameLook)
  {
    LoadFrameStyle(frameStyle);
    shouldCloseUnfocus = true;
    LoadStyles(frameLook);
    Measure({}, {});

    Color hsv = Color(color).ToHSV();
    svSlider = GetComponentByName<SatValSlider>("CPW#SatValSlider");

    hSlider = GetComponentByName<HueSlider>("CPW#HueSlider");
    rTextBox = GetComponentByName<TextBox>("CPW#RTextBox");
    gTextBox = GetComponentByName<TextBox>("CPW#GTextBox");
    bTextBox = GetComponentByName<TextBox>("CPW#BTextBox");
    hTextBox = GetComponentByName<TextBox>("CPW#HTextBox");
    sTextBox = GetComponentByName<TextBox>("CPW#STextBox");
    vTextBox = GetComponentByName<TextBox>("CPW#VTextBox");
    hexTextBox = GetComponentByName<TextBox>("CPW#HexTextBox");
    colorDisplay = GetComponentByName<Component>("CPW#ColorDisplay");

    using namespace std::placeholders;

    hSlider->SetOnValueChangeCallback(BIND_MEMBER_FUNC(SliderChanged));
    svSlider->SetOnSatChangeCallback(BIND_MEMBER_FUNC(SliderChanged));
    svSlider->SetOnValChangeCallback(BIND_MEMBER_FUNC(SliderChanged));
    rTextBox->SetOnTextChangedCallback(BIND_MEMBER_FUNC(RGBTextBoxChanged));
    gTextBox->SetOnTextChangedCallback(BIND_MEMBER_FUNC(RGBTextBoxChanged));
    bTextBox->SetOnTextChangedCallback(BIND_MEMBER_FUNC(RGBTextBoxChanged));
    hTextBox->SetOnTextChangedCallback(BIND_MEMBER_FUNC(HSVTextBoxChanged));
    sTextBox->SetOnTextChangedCallback(BIND_MEMBER_FUNC(HSVTextBoxChanged));
    vTextBox->SetOnTextChangedCallback(BIND_MEMBER_FUNC(HSVTextBoxChanged));
    hexTextBox->SetOnTextChangedCallback(BIND_MEMBER_FUNC(HexTextBoxChanged));

    // Make textboxes and other stuff update
    UpdateColor(color, InputChangeType::NONE, true, true);
    SetPosition(pos);

  }
  ColorPickerWindow::ColorPickerWindow()
    : ColorPickerWindow {{0,0}, {1,1,1}}
  {
  }

  void ColorPickerWindow::UpdateColor(const Color& avColor, InputChangeType type, bool rgb, bool callback)
  {
    Color prevRGB = color;
    color = avColor;
    Color hsv = avColor;
    if(!rgb)
      color.ToRGB();
    else
      hsv = hsv.ToHSV();

    if(color == prevRGB)
      return;

    ComponentStyle& s = colorDisplay->GetStyle("normal");
    s.SetColor("backgroundColor", color);
    svSlider->SetHue(hsv.h);

    if(type != InputChangeType::RGB_TEXTBOX)
    {
      rTextBox->SetText(std::to_string((int)(255*color.r)));
      gTextBox->SetText(std::to_string((int)(255*color.g)));
      bTextBox->SetText(std::to_string((int)(255*color.b)));
    }
    if(type != InputChangeType::HSV_TEXTBOX)
    {
      hTextBox->SetText(std::to_string((int)(255*hsv.h)));
      sTextBox->SetText(std::to_string((int)(255*hsv.s)));
      vTextBox->SetText(std::to_string((int)(255*hsv.v)));
    }
    if(type != InputChangeType::SLIDER)
    {
      hSlider->SetValue(hsv.h, false);
      svSlider->SetSat(hsv.s, false);
      svSlider->SetVal(hsv.v, false);
    }
    if(type != InputChangeType::HEX_TEXTBOX)
    {
      hexTextBox->SetText(LogUtils::DecToHex(((int)(255*color.r) << 16) | ((int)(255*color.g) << 8)  | (int)(255*color.b),6));
    }
    if(callback)
    {
      CallOnColorChangeCallback(prevRGB, color);
    }
  }

  void ColorPickerWindow::SliderChanged(Component* component, float oldValue, float newValue)
  {
    UpdateColor({hSlider->GetValue(), svSlider->GetSat(), svSlider->GetVal()}, InputChangeType::SLIDER, false, true);
  }

  void ColorPickerWindow::RGBTextBoxChanged(Component* textBox, const std::string& oldText, const std::string& newText)
  {
    float r = atof(rTextBox->GetText().c_str())/255.0f;
    float g = atof(gTextBox->GetText().c_str())/255.0f;
    float b = atof(bTextBox->GetText().c_str())/255.0f;

    Math::Clamp(&r,0.0f,1.0f);
    Math::Clamp(&g,0.0f,1.0f);
    Math::Clamp(&b,0.0f,1.0f);

    UpdateColor(Color(r, g, b), InputChangeType::RGB_TEXTBOX, true, true);
  }

  void ColorPickerWindow::HSVTextBoxChanged(Component* textBox, const std::string& oldText, const std::string& newText)
  {
    float h = atoi(hTextBox->GetText().c_str())/255.0f;
    float s = atoi(sTextBox->GetText().c_str())/255.0f;
    float v = atoi(vTextBox->GetText().c_str())/255.0f;
    Math::Clamp(&h,0.0f,1.0f);
    Math::Clamp(&s,0.0f,1.0f);
    Math::Clamp(&v,0.0f,1.0f);
    UpdateColor({h, s, v}, InputChangeType::HSV_TEXTBOX, false, true);
  }

  void ColorPickerWindow::HexTextBoxChanged(Component* textBox, const std::string& oldText, const std::string& newText)
  {
    UpdateColor(Color(0xff000000 | LogUtils::HexToDec(hexTextBox->GetText())), InputChangeType::HEX_TEXTBOX, true, true);
  }

  void ColorPickerWindow::SetOnColorChangeCallback(OnColorChangeCallback callback)
  {
    onColorChangeCallback = callback;
  }

  void ColorPickerWindow::CallOnColorChangeCallback(const Color& previous, const Color& current)
  {
    if(onColorChangeCallback)
      onColorChangeCallback(previous, current);
  }

  void ColorPickerWindow::SetColor(const Color& color)
  {
    UpdateColor(color, InputChangeType::NONE, true, false);
  }

  const Color& ColorPickerWindow::GetColor() const
  {
    return color;
  }
}
