#include "Component.h"

#include <utils/AABBUtils.h>
#include <utils/UUID.h>
#include <functional>
#include <cstdlib>
#include <graphics/gui/Container.h>


namespace Greet
{

  REGISTER_COMPONENT_DEFINITION(Component);

  Component::Component(const std::string& name, Component* parent)
    : parent{parent}, size{},
    m_isFocusable{false},isFocused{false},isHovered{false},isChildFocused{false}, childChangedFocus{false}, pos{0,0}, pressed{false}, name{name}
  {
    currentStyle = &normalStyle;
  }

  Component::Component(const XMLObject& xmlObject, Component* parent)
    : Component{GUIUtils::GetStringFromXML(xmlObject,"name", xmlObject.GetName() + "#" + LogUtils::DecToHex(UUID::GetInstance().GetUUID(),8)), parent}
  {
    size = GUIUtils::GetComponentSizeFromXML(xmlObject, "width", "height", {});
    normalStyle.Load("",xmlObject);
    hoverStyle.Load("hover",xmlObject,&normalStyle);
    pressStyle.Load("press",xmlObject,&normalStyle);
  }

  void Component::Measure()
  {
    Container* container = dynamic_cast<Container*>(parent);
    if(size.widthType != ComponentSize::Type::WEIGHT || (container && container->IsVertical() ))
    {
      if(size.widthType == ComponentSize::Type::WRAP)
        size.size.w = GetWrapSize().w;
      else
        size.size.w = size.value.w;
    } 

    if(size.heightType != ComponentSize::Type::WEIGHT || (container && container->IsVertical() ))
    {
      if(size.heightType == ComponentSize::Type::WRAP)
        size.size.h = GetWrapSize().h;
      else
        size.size.h = size.value.h;
    } 
  }

  void Component::MeasureFill()
  {
    MeasureFill(guiScene->GetWidth(), guiScene->GetHeight(), 1, true);
  }

  void Component::MeasureFill(float parentEmptyWidth, float parentEmptyHeight, float parentTotalWeight, bool vertical)
  {
    // Width
    if(size.widthType == ComponentSize::Type::WEIGHT)
    {
      if(vertical)
        size.size.w = parentEmptyWidth;
      else
        size.size.w = parentEmptyWidth * size.value.w / parentTotalWeight;
    }
    // else we have already set the size in Measure


    // Height
    if(size.heightType == ComponentSize::Type::WEIGHT)
    {
      if(!vertical)
        size.size.h = parentEmptyHeight;
      else
        size.size.h = parentEmptyHeight * size.value.h / parentTotalWeight;
    }
    // else we have already set the size in Measure

    OnMeasured();
  }

  Vec2 Component::GetMeasureFillSize()
  {
    return GetContentSize();
  }

  float Component::GetMeasureTotalWeight()
  {
    return 1;
  }

  void Component::Remeasure()
  {
    if(!parent)
    {
      Measure();
      MeasureFill();
    }
    else
    {
      GetRootNode()->Remeasure();
    }
  }

  // Push translation to renderer
  void Component::PreRender(GUIRenderer* renderer, const Vec2& translation) const
  {
    renderer->PushMatrix(Mat3::Translate(translation));

    // Border around Component 
    if (currentStyle->hasBorderColor)
      //renderer->SubmitRect(pos + Vec2(0,0), size, currentStyle->borderColor, false);
      renderer->SubmitRoundedRect(pos+Vec2(0,0),size.size, currentStyle->borderColor, currentStyle->borderRadius, currentStyle->roundedPrecision, false);

    // Component background
    if (currentStyle->hasBackgroundColor)
      renderer->SubmitRoundedRect(pos + currentStyle->border.LeftTop(), size.size-GetBorder().LeftTop()-GetBorder().RightBottom(), currentStyle->backgroundColor, currentStyle->radius,currentStyle->roundedPrecision,false);
  }

  // Render component
  void Component::RenderHandle(GUIRenderer* renderer) const
  {
    // Render rest of component
    Render(renderer);
  }

  // Pop translation from renderer
  void Component::PostRender(GUIRenderer* renderer) const
  {
    renderer->PopMatrix();
  }

  void Component::UpdateHandle(float timeElapsed)
  {
    size.size = Vec2(GetWidth(), GetHeight());
    Update(timeElapsed);
  }

  void Component::OnMousePressed(MousePressEvent& event, const Vec2& translatedPos)
  {
    if(m_isFocusable)
    {
      if(!isFocused)
      {
        guiScene->RequestFocus(this);
      }
      MousePressed(event,translatedPos);
    }
  }

  void Component::OnMouseMoved(MouseMoveEvent& event, const Vec2& translatedPos)
  {
    if(m_isFocusable)
    {
      if(IsMouseInside(pos+translatedPos))
      {
        if(!isHovered)
        {
          isHovered = true;
          MouseEntered();
        }
        MouseMoved(event,translatedPos);
      }
      else if(isHovered)
      {
        isHovered = false;
        MouseExited();
      }
    }
  }
  void Component::SetOnClickCallback(OnClickCallback callback)
  {
    onClickCallback = callback;
  }

  void Component::SetOnPressCallback(OnPressCallback callback)
  {
    onPressCallback = callback;
  }

  void Component::SetOnReleaseCallback(OnReleaseCallback callback)
  {
    onReleaseCallback = callback;
  }

  void Component::CallOnClickCallback()
  {
    if(onClickCallback)
      onClickCallback(this);
  }

  void Component::CallOnPressCallback()
  {
    if(onPressCallback)
      onPressCallback(this);
  }

  void Component::CallOnReleaseCallback()
  {
    if(onReleaseCallback)
      onReleaseCallback(this);
  }

  void Component::MousePressed(MousePressEvent& event, const Vec2& translatedPos)
  {
    if(event.GetButton() == GLFW_MOUSE_BUTTON_1)
    {
      pressed = true;
      CallOnPressCallback();
    }
  }

  void Component::MouseReleased(MouseReleaseEvent& event, const Vec2& translatedPos)
  {
    if(event.GetButton() == GLFW_MOUSE_BUTTON_1)
    {
      if(pressed)
      {
        if(IsMouseInside(translatedPos+pos))
          CallOnClickCallback();
        CallOnReleaseCallback();
      }
      pressed = false;
    }
  }

  void Component::OnFocused()
  {
    isFocused = true;
    Focused();
  }

  void Component::OnUnfocused()
  {
    isFocused = false;
    Unfocused();
  }

  bool Component::UsingMouse()
  {
    return pressed;
  }

  Vec2 Component::GetPosition() const
  {
    return pos;
  }

  Vec2 Component::GetRealPosition() const
  {
    return pos + GetMargin().LeftTop() + (parent ? parent->GetTotalPadding()+parent->GetRealPosition() : Vec2(0,0));
  }

  Vec2 Component::GetWrapSize() const
  {
    return Vec2(100,100);
  }

  Component* Component::GetParent() const
  {
    return parent;
  }

  void Component::SetPosition(const Vec2& pos) 
  {
    this->pos = pos;
  }

  Vec2 Component::GetSize() const
  {
    return size.size;
  }

  float Component::GetWidth() const 
  {
    return size.size.w;
  }

  float Component::GetHeight() const 
  {
    return size.size.h;
  }

  ComponentSize::Type Component::GetWidthSizeType() const
  {
    return size.widthType;
  }

  ComponentSize::Type Component::GetHeightSizeType() const
  {
    return size.heightType;
  }

  Component& Component::SetWidth(float width)
  {
    size.value.w = width;
    Remeasure();
    return *this;
  }

  Component& Component::SetHeight(float height)
  {
    size.value.h = height;
    Remeasure();
    return *this;
  }

  Component& Component::SetWidthSizeType(ComponentSize::Type width)
  {
    size.widthType = width;
    Remeasure();
    return *this;
  }

  Component& Component::SetHeightSizeType(ComponentSize::Type height)
  {
    size.heightType = height;
    Remeasure();
    return *this;
  }

  Component& Component::SetSize(float width, float height, ComponentSize::Type widthType, ComponentSize::Type heightType, bool remeasure)
  {
    size.value.w = width;
    size.value.h = height;
    size.widthType = widthType;
    size.heightType = heightType;
    if(remeasure)
      Remeasure();
    return *this;
  }

  void Component::SetGUIScene(GUIScene* scene)
  {
    guiScene = scene;
  }

  const Style& Component::GetNormalStyle() const
  {
    return normalStyle;
  }

  const Style& Component::GetHoverStyle() const
  {
    return hoverStyle;
  }

  const Style& Component::GetPressStyle() const
  {
    return pressStyle;
  }

  Component& Component::SetNormalStyle(const Style& style)
  {
    normalStyle = style;
    if(currentStyle == &normalStyle)
      Remeasure();
    return *this;
  }
  Component& Component::SetHoverStyle(const Style& style)
  {
    hoverStyle = style;
    if(currentStyle == &hoverStyle)
      Remeasure();
    return *this;
  }

  Component& Component::SetPressStyle(const Style& style)
  {
    pressStyle = style;
    if(currentStyle == &pressStyle)
      Remeasure();
    return *this;
  }

  const Vec2& Component::GetSizeValue() const
  {
    return size.value;
  }

  Component* Component::GetComponentByNameNoCast(const std::string& name)
  {
    if(name == this->name)
      return this;
    return nullptr;
  }

  bool Component::IsMouseInside(const Vec2& parentMouse) const
  {
    return AABBUtils::PointInsideBox(parentMouse, pos, size.size);
  }

  Component* Component::GetRootNode()
  {
    Component* par = this;
    while(par->GetParent())
      par = par->GetParent();
    return par;
  }

  uint Component::GetComponentDepth() const
  {
    uint depth = 1;
    const Component* par = this; 
    while(par->GetParent())
    {
      depth++;
      par = par->GetParent();
    }
    return depth;
  }

  Vec2 Component::GetTotalPadding() const
  {
    return GetPadding().LeftTop() + GetBorder().LeftTop();
  }

  Vec2 Component::GetContentSize() const
  {
    return size.size - GetPadding().GetSize() - GetBorder().GetSize();
  }

  const std::string& Component::GetName() const
  {
    return name;
  }  

  const TLBR& Component::GetMargin() const
  {
    return currentStyle->margin;
  }

  const TLBR& Component::GetPadding() const
  {
    return currentStyle->padding;
  }

  const TLBR& Component::GetBorder() const
  {
    return currentStyle->border;
  }
}
