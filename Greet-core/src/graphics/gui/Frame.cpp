#include "Frame.h"

#include <graphics/gui/GUIUtils.h>
#include <input/InputDefines.h>
#include <utils/Utils.h>
#include <utils/AABBUtils.h>

namespace Greet {

  REGISTER_FRAME_DEFINITION(Frame);

  uint8_t Frame::RESIZING_LEFT = BIT(0);
  uint8_t Frame::RESIZING_RIGHT = BIT(1);
  uint8_t Frame::RESIZING_TOP = BIT(2);
  uint8_t Frame::RESIZING_BOTTOM = BIT(3);
  int Frame::RESIZING_MARGIN = 5;

  Frame::Frame()
    : Frame(XMLObject())
  {

  }

  // These are usually the top element so no parent is needed
  Frame::Frame(const XMLObject& object)
    : Container(object, nullptr), shouldCloseUnfocus{false}, m_stayInsideWindow{true}
  {
    if(object.HasAttribute("styling"))
    {
      std::string file = object.GetAttribute("styling");
      styleFile = MetaFile{file};
      LoadFrameStyle(styleFile);
    }
    m_resizableFlags = RESIZING_LEFT | RESIZING_RIGHT | RESIZING_TOP | RESIZING_BOTTOM;
    m_resizing = 0;
    minSize = Vec2f(100, 100);

    pos.x = GUIUtils::GetFloatFromXML(object, "x", 0);
    pos.y = GUIUtils::GetFloatFromXML(object, "y", 0);

    minSize.w = GUIUtils::GetFloatFromXML(object,"minWidth", 100);
    minSize.h = GUIUtils::GetFloatFromXML(object,"minHeight", 100);

    // This can still look better
    if(!GUIUtils::GetBooleanFromXML(object,"resizeLeft", false))
      m_resizableFlags &= ~RESIZING_LEFT;
    if(!GUIUtils::GetBooleanFromXML(object,"resizeRight", false))
      m_resizableFlags &= ~RESIZING_RIGHT;
    if(!GUIUtils::GetBooleanFromXML(object,"resizeTop", false))
      m_resizableFlags &= ~RESIZING_TOP;
    if(!GUIUtils::GetBooleanFromXML(object,"resizeBottom", false))
      m_resizableFlags &= ~RESIZING_BOTTOM;
  }

  Frame::~Frame()
  {
  }

  bool Frame::IsHoverResize(const Vec2f& mousePos)
  {
    if ((m_resizableFlags & RESIZING_LEFT) != 0 && mousePos.x >= -RESIZING_MARGIN && mousePos.x < RESIZING_MARGIN)
    {
      return true;
    }
    else if ((m_resizableFlags & RESIZING_RIGHT) != 0 && mousePos.x >= width.size - RESIZING_MARGIN && mousePos.x < width.size + RESIZING_MARGIN)
    {
      return true;
    }
    if ((m_resizableFlags & RESIZING_TOP) != 0 && mousePos.y >= -RESIZING_MARGIN && mousePos.y < RESIZING_MARGIN)
    {
      return true;
    }
    else if ((m_resizableFlags & RESIZING_BOTTOM) != 0 && mousePos.y >= height.size - RESIZING_MARGIN && mousePos.y < height.size + RESIZING_MARGIN)
    {
      return true;
    }
    return false;
  }

  void Frame::SetResizeFlags(const Vec2f& mousePos)
  {
    m_resizingFlags = 0;
    if ((m_resizableFlags & RESIZING_LEFT) != 0 && mousePos.x >= -RESIZING_MARGIN && mousePos.x < RESIZING_MARGIN)
    {
      m_resizingFlags |= RESIZING_LEFT;
    }
    else if ((m_resizableFlags & RESIZING_RIGHT) != 0 && mousePos.x >= width.size - RESIZING_MARGIN && mousePos.x < width.size + RESIZING_MARGIN)
    {
      m_resizingFlags |= RESIZING_RIGHT;
    }
    if ((m_resizableFlags & RESIZING_TOP) != 0 && mousePos.y >= -RESIZING_MARGIN && mousePos.y < RESIZING_MARGIN)
    {
      m_resizingFlags |= RESIZING_TOP;
    }
    else if ((m_resizableFlags & RESIZING_BOTTOM) != 0 && mousePos.y >= height.size - RESIZING_MARGIN && mousePos.y < height.size + RESIZING_MARGIN)
    {
      m_resizingFlags |= RESIZING_BOTTOM;
    }
    m_resizing = m_resizingFlags != 0;
  }

  void Frame::Resize(const Vec2f& mousePos)
  {
    Vec2f oldSize = GetSizeValue();
    if (m_resizingFlags & RESIZING_LEFT)
    {
      pos.x = m_posOrigin.x - (m_clickPos.x - mousePos.x);
      width.value = m_sizeOrigin.x + (m_clickPos.x - mousePos.x);
      width.type = GUISize::Type::Pixels;
      if (width.value < minSize.w)
      {
        pos.x = m_posOrigin.x + (m_sizeOrigin.x - minSize.w);
        width.value = minSize.w;
      }
    }
    else if (m_resizingFlags & RESIZING_RIGHT)
    {
      width.value = m_sizeOrigin.x - (m_clickPos.x - mousePos.x);
      width.type = GUISize::Type::Pixels;
      if (width.value < minSize.w)
        width.value = minSize.w;
    }
    if (m_resizingFlags & RESIZING_TOP)
    {
      pos.y = m_posOrigin.y - (m_clickPos.y - mousePos.y);
      height.value = m_sizeOrigin.y + (m_clickPos.y - mousePos.y);
      height.type = GUISize::Type::Pixels;
      if (height.value < minSize.h)
      {
        pos.y = m_posOrigin.y + (m_sizeOrigin.y - minSize.h);
        height.value = minSize.h;
      }
    }
    else if (m_resizingFlags & RESIZING_BOTTOM)
    {
      height.value = m_sizeOrigin.y - (m_clickPos.y - mousePos.y);
      height.type = GUISize::Type::Pixels;
      if (height.value < minSize.h)
        height.value = minSize.h;
    }
    ResizeScreenClamp();
    if(oldSize != Vec2f{width.value, height.value})
    {
      Remeasure();
      GetStyle("normal").SetGUISize("width", width);
      GetStyle("normal").SetGUISize("height", height);
    }
  }

  void Frame::ResizeScreenClamp()
  {
    if (m_stayInsideWindow)
    {
      if (pos.x < 0)
      {
        pos.x = 0;
        width.value = m_posOrigin.x + m_sizeOrigin.x;
      }
      else if (pos.x > guiScene->GetWidth() - width.size)
      {
        width.value = guiScene->GetWidth() - m_posOrigin.x;
      }
      if (pos.y < 0)
      {
        pos.y = 0;
        height.value = m_posOrigin.y + m_sizeOrigin.y;
      }
      else if (pos.y > guiScene->GetHeight() - height.size)
        height.value = guiScene->GetHeight() - m_posOrigin.y;
    }
  }

  void Frame::OnMousePressEventHandler(MousePressEvent& event, const Vec2f& componentPos)
  {
    Vec2f translatedPos = event.GetPosition() - componentPos;
    if (event.GetButton() == GREET_MOUSE_1)
    {
      if(IsHoverResize(translatedPos))
      {
        m_posOrigin = pos;
        m_sizeOrigin = {width.size, height.size};
        m_clickPos = translatedPos;
        SetResizeFlags(translatedPos);
        Component::OnMousePressEventHandler(event, componentPos);
      }
      else
      {
        Container::OnMousePressEventHandler(event, componentPos);
      }
    }
    else
      Container::OnMousePressEventHandler(event, componentPos);
  }

  void Frame::OnMouseReleaseEventHandler(MouseReleaseEvent& event, const Vec2f& componentPos)
  {
    if (event.GetButton() == GREET_MOUSE_1)
      m_resizing = false;
    Container::OnMouseReleaseEventHandler(event, componentPos);
  }

  void Frame::OnMouseMoveEventHandler(MouseMoveEvent& event, const Vec2f& componentPos)
  {
    if (m_resizing)
      Resize(event.GetPosition() - m_posOrigin);
    else if(!IsHoverResize(event.GetPosition() - componentPos))
      Container::OnMouseMoveEventHandler(event, componentPos);
  }

  void Frame::SetGUIMouseListener(GUIMouseListener* listener)
  {
    m_mouseListener = listener;
  }

  void Frame::OnViewportResize(int width, int height)
  {
    Remeasure();
  }

  bool Frame::IsMouseInside(const Vec2f& mousePos) const
  {
    Vec2f resizeMargin = Vec2f(RESIZING_MARGIN, RESIZING_MARGIN);
    return AABBUtils::PointInsideBox(mousePos, -resizeMargin, GetSize() + resizeMargin*2);
  }

  void Frame::SetPosition(const Vec2f& pos)
  {
    if(m_stayInsideWindow)
    {
      Vec2f p = pos;
      if(p.x + width.size > guiScene->GetWidth())
      {
        p.x = guiScene->GetWidth() - width.size;
      }
      if(p.y + height.size > guiScene->GetHeight())
      {
        p.y = guiScene->GetHeight() - height.size;
      }
      Component::SetPosition(p);
      return;
    }
    Component::SetPosition(pos);
  }

  void Frame::ChildChangedFocus(bool focused)
  {
    if(!GetParent() && !focused && shouldCloseUnfocus)
    {
      guiScene->RemoveFrameQueued(name);
    }
  }
}
