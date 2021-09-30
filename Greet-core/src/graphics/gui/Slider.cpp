#include "Slider.h"

#include <input/InputDefines.h>
#include <math/Maths.h>

#include <sstream>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(Slider);

  Slider::Slider(const std::string& name, Component* parent)
    : Component{name, parent}
  {
    AddStyleVariables(StylingVariables{.bools = {{"vertical", &vertical}, {"reverse", &reverse}}});

    sliderComponent = new Component{name+"#SliderIndicator", this, "SliderIndicator"};

    minValue = 0;
    maxValue = 100;
    stepSize = 0;

    SetValue((maxValue-minValue)/2.0f);
  }

  Slider::Slider(const XMLObject& xmlObject, Component* parent)
    : Component(xmlObject, parent)
  {
    AddStyleVariables(StylingVariables{.bools = {{"vertical", &vertical}, {"reverse", &reverse}}});
    LoadStyles(xmlObject);

    minValue = GUIUtils::GetFloatFromXML(xmlObject, "minValue", 0.0f);
    maxValue = GUIUtils::GetFloatFromXML(xmlObject, "maxValue", 100.0f);
    stepSize = GUIUtils::GetFloatFromXML(xmlObject, "stepSize", 0.0f);

    if(maxValue - minValue < 0)
    {
      Log::Warning("Min value in slider is greater than max value");
      std::swap(minValue,maxValue);
    }

    if(stepSize < 0)
    {
      stepSize = -stepSize;
      Log::Warning("Step size in Slider is negative");
    }

    if(stepSize > 0)
      snapSlider = true;


    // Bit of a hack to make the initial value correct
    minPos = 0;
    maxPos = 1;

    clampSlider = GUIUtils::GetBooleanFromXML(xmlObject, "indicatorInside", false);
    if(xmlObject.GetObjectCount() > 0)
    {
      sliderComponent = ComponentFactory::GetComponent(xmlObject.GetObject(0), this);
    }
    else
    {
      sliderComponent = new Component{name+"#SliderIndicator", this, "SliderIndicator"};
    }

    // Default defaultValue should be in the middle
    SetValue(GUIUtils::GetFloatFromXML(xmlObject, "defaultValue", (maxValue-minValue)/2.0f));
  }

  void Slider::Measure(const Vec2f& emptyParentSpace, const Vec2f& percentageFill)
  {
    Component::Measure(emptyParentSpace, percentageFill);
    sliderComponent->Measure(GetContentSize(),{1,1});
  }

  void Slider::OnMeasured()
  {
    // vi will be the y value of the sizes if vertical and x value if not
    float& sliderDirSize = vertical ? height.size : width.size;
    uint32_t vi = vertical ? 1 : 0;

    if(clampSlider)
    {
      minPos = sliderComponent->GetSize()[vi]/2;
      maxPos = sliderDirSize - GetBorder().GetSize()[vi]-minPos;
    }
    else
    {
      minPos = 0;
      maxPos = sliderDirSize;
    }
    if(reverse)
      std::swap(minPos, maxPos);
  }

  void Slider::Render(GUIRenderer* renderer) const
  {
    float sliderPos = GetSliderPosFromValue(value);
    Vec2f sliderOffset = pos + GetTotalPadding() + sliderComponent->GetMargin().LeftTop() - sliderComponent->GetSize()/2;
    if(vertical)
      sliderComponent->PreRender(renderer,  sliderOffset + Vec2f{GetContentSize().w/2, sliderPos});
    else
      sliderComponent->PreRender(renderer,  sliderOffset + Vec2f{sliderPos, GetContentSize().h/2});
    sliderComponent->RenderHandle(renderer);
    sliderComponent->PostRender(renderer);
  }

  void Slider::OnEvent(Event& event, const Vec2f& componentPos)
  {
    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      if(e.GetButton() == GREET_MOUSE_1 && pressed)
      {
        Vec2f translatedPos = e.GetPosition() - componentPos;
        float oldValue = value;
        if(vertical)
          SetValue(GetSliderValueFromPos(translatedPos.y));
        else
          SetValue(GetSliderValueFromPos(translatedPos.x));
        if(oldValue != value)
          CallOnValueChangeCallback(oldValue, value);
      }
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      MouseMoveEvent& e = static_cast<MouseMoveEvent&>(event);
      if(pressed)
      {
        Vec2f translatedPos = e.GetPosition() - componentPos;
        float oldValue = value;
        if(vertical)
          SetValue(GetSliderValueFromPos(translatedPos.y));
        else
          SetValue(GetSliderValueFromPos(translatedPos.x));
        if(oldValue != value)
          CallOnValueChangeCallback(oldValue, value);
      }
    }

  }

  void Slider::SetOnValueChangeCallback(OnValueChangeCallback callback)
  {
    onValueChangeCallback = callback;
    CallOnValueChangeCallback(value, value);
  }

  void Slider::CallOnValueChangeCallback(float oldValue, float newValue)
  {
    if(onValueChangeCallback)
      onValueChangeCallback(this, oldValue, newValue);
  }

  Slider& Slider::SetVertical(bool vertical)
  {
    this->vertical = vertical;
    return *this;
  }

  void Slider::LoadFrameStyle(const MetaFile& metaFile)
  {
    Component::LoadFrameStyle(metaFile);
    sliderComponent->LoadFrameStyle(metaFile);
  }

  Component* Slider::GetSliderComponent()
  {
    return sliderComponent;
  }

  float Slider::GetValue() const
  {
    return value;
  }

  float Slider::GetMinValue() const
  {
    return minValue;
  }

  float Slider::GetMaxValue() const
  {
    return maxValue;
  }

  void Slider::SetValue(float afValue, bool abCallback)
  {
    Math::Clamp(&afValue, minValue, maxValue);
    float oldValue = value;
    value = afValue;
    if(snapSlider)
      value = GetSnappedSlider(afValue);

    if(abCallback && value != oldValue)
      CallOnValueChangeCallback(oldValue, value);
  }

  float Slider::GetSnappedSlider(float sliderValue) const
  {
    return Math::RoundClose(sliderValue,stepSize);
  }

  float Slider::GetSliderValueFromPos(float pos) const
  {
    return ((pos - minPos) / (maxPos - minPos)) * (maxValue-minValue) + minValue;
  }

  float Slider::GetSliderPosFromValue(float value) const
  {
    return (value - minValue) / (maxValue - minValue) * (maxPos - minPos) + minPos;
  }
}
