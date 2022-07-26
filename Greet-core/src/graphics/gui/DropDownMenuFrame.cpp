#include "DropDownMenuFrame.h"

#include <graphics/gui/Label.h>

namespace Greet
{
  DropDownMenuFrame::DropDownMenuFrame(const XMLObject& xmlObject, const std::vector<std::string>& items)
    : Frame{XMLObject{"DropDownMenu", {{"name", "dropDownMenuFrame"}}, ""}}, items{items}
  {
    SetStyleVariables({
      .colors=
      {
        {"dropBackgroundColor", &dropBackgroundColor},
        {"dropBorderColor", &dropBorderColor},
        {"dropTextColor", &text.color}
      },
      .tlbrs=
      {
        {"dropPadding", &dropPadding},
        {"dropBorder", &dropBorder}
      },
      .floats
      {
        {{"dropHeight", &dropHeight}},
      },
      .fonts={{"dropFont", &text.font}},
    });

    isFocusable = true;
    shouldCloseUnfocus = true;

    std::string grav = xmlObject.GetAttribute("dropGravity", "center");
    if(grav == "top")
      text.gravity = Text::Gravity::Top;
    else if(grav == "bottom")
      text.gravity = Text::Gravity::Bottom;
    else
      text.gravity = Text::Gravity::Center;

    std::string sAlign = xmlObject.GetAttribute("dropAlign", "left");
    if(sAlign == "center")
      text.align = Text::Align::Center;
    else if(sAlign == "right")
      text.align = Text::Align::Right;
    else
      text.align = Text::Align::Left;
    text.overlapMode = Text::OverlapMode::Ignore;

    height = GUISize{0, (dropBorder.GetHeight() + text.GetWrapHeight(0)) * items.size(), GUISize::Type::Pixels};
    width = GUISize{0, 50, GUISize::Type::Pixels};
  }

  void DropDownMenuFrame::Render(GUIRenderer* renderer) const
  {
    Text txt = text;
    for(int i = 0; i < items.size(); i++)
    {
      txt.str = items[i];
      if(i == pressPos)
        RenderItem(renderer, txt, i, "press");
      else if(i == hoverPos)
        RenderItem(renderer, txt, i, "hover");
      else
        RenderItem(renderer, txt, i, "normal");
    }
  }

  void DropDownMenuFrame::RenderItem(GUIRenderer* renderer, const Text& text, int i, const std::string& styling) const
  {
    float dropHeight = GetStyle("normal").GetFloat("dropHeight");
    Color dropBorderColor = GetStyle(styling).GetColor("dropBorderColor");
    Color dropBackgroundColor = GetStyle(styling).GetColor("dropBackgroundColor");
    TLBR dropBorder = GetStyle(styling).GetTLBR("dropBorder");
    TLBR dropPadding = GetStyle(styling).GetTLBR("dropPadding");

    renderer->PushTranslation(Vec2f{0, (float)(dropHeight * i)});
    renderer->DrawRect({}, {GetWidth(), dropHeight}, dropBorderColor, false);
    renderer->DrawRect(dropBorder.LeftTop(), Vec2f{GetWidth(), dropHeight} - dropBorder.GetSize(), dropBackgroundColor, false);
    text.Render(renderer, dropPadding.LeftTop() + dropBorder.LeftTop(), Vec2f{GetWidth(), dropHeight} - dropPadding.GetSize() - dropBorder.GetSize());
    renderer->PopTranslation();
  }

  void DropDownMenuFrame::OnEvent(Event& event, const Vec2f& componentPos)
  {
    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      pressPos = floor((e.GetY() - componentPos.y) / dropHeight);
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_RELEASE))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      if(pressed && IsMouseInside(e.GetPosition() - componentPos))
      {
        int index = floor((e.GetY() - componentPos.y) / dropHeight);
        if(pressPos == index)
        {
          ASSERT(index >= 0 && index < items.size(), "DropDownMenu index is out of bounds (index=%s, size=%s)", index, items.size());
          CallOnSelectionChanged(index);
        }
      }
      pressPos = -1;
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      MouseMoveEvent& e = static_cast<MouseMoveEvent&>(event);
      hoverPos = floor((e.GetY() - componentPos.y) / dropHeight);
    }
    Frame::OnEvent(event, componentPos);
  }

  void DropDownMenuFrame::Measure(const Vec2f& emptyParentSpace, const Vec2f& percentageFill)
  {
    width.size = width.value;
    height.size = dropHeight * items.size();
  }

  void DropDownMenuFrame::SetDropDownItems(const std::vector<std::string>& items)
  {
    this->items = items;
    Remeasure();
  }

  void DropDownMenuFrame::SetOnSelectionChangedCallback(OnSelectionChanged callback)
  {
    this->callback = callback;
  }

  void DropDownMenuFrame::CallOnSelectionChanged(int index)
  {
    if(callback)
      callback(index);

    // Unfocus this frame to close it
    RequestUnfocus();
  }
}
