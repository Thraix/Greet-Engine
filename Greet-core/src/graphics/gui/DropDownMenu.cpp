#include "DropDownMenu.h"

#include <algorithm>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(DropDownMenu);

  DropDownMenu::DropDownMenu(const XMLObject& object, Component* parent)
    : Component(object, parent)
  {
    AddStyleVariables(StylingVariables{
        .colors={
          {"textColor", &text.color}},
        .fonts={{"font", &text.font}}
        });
    LoadStyles(object);

    std::string grav = object.GetAttribute("gravity", "center");
    if(grav == "top")
      text.gravity = Text::Gravity::Top;
    else if(grav == "bottom")
      text.gravity = Text::Gravity::Bottom;
    else
      text.gravity = Text::Gravity::Center;

    std::string sAlign = object.GetAttribute("align", "left");
    if(sAlign == "center")
      text.align = Text::Align::Center;
    else if(sAlign == "right")
      text.align = Text::Align::Right;
    else
      text.align = Text::Align::Left;
    text.overlapMode = Text::OverlapMode::Ignore;
    for(auto& item : object.GetObjects())
    {
      if(item.GetName() == "DropDownItem")
      {
        items.emplace_back(item.GetText());
      }
    }
    if(items.empty())
      this->items.emplace_back("No items");

    frame = new DropDownMenuFrame(object, items);
    frame->SetOnSelectionChangedCallback(BIND_MEMBER_FUNC(CallOnSelectionChanged));
  }

  void DropDownMenu::Render(GUIRenderer* renderer) const
  {
    Text txt = text;
    txt.str = items[selectedIndex];
    renderer->PushTranslation(pos + GetTotalPadding());
    txt.Render(renderer, GetContentSize());
    renderer->PopTranslation();
    float size = text.font.GetSize() / 2.0f;
    Vec2f center = pos + GetSize() - Vec2f{GetHeight(), GetHeight()} / 2.0f;
    if(frame->IsFocused())
    {
      renderer->DrawTriangle(center + Vec2f(0, -size * 0.75),
                             center + Vec2f{-size * 0.866f, size * 0.75f},
                             center + Vec2f{size * 0.866f, size * 0.75f}, text.color, false);
    }
    else
    {
      renderer->DrawTriangle(center + Vec2f(0, size * 0.75),
                             center + Vec2f{-size * 0.866f, -size * 0.75f},
                             center + Vec2f{size * 0.866f, -size * 0.75f}, text.color, false);
    }
  }

  void DropDownMenu::OnMouseReleaseEventHandler(MouseReleaseEvent& event, const Vec2f& componentPos)
  {
    if(pressed && IsMouseInside(event.GetPosition() - componentPos))
    {
      if(frame->IsFocused())
      {
        guiScene->RequestFocusQueued(nullptr);
      }
      else
      {
        frame->SetPosition(componentPos + Vec2f{0, GetHeight()});
        frame->SetWidth(GetWidth());
        guiScene->AddFrameQueued(frame);
        guiScene->RequestFocusQueued(frame);
      }
    }
  }

  void DropDownMenu::LoadFrameStyle(const MetaFile& metaFile)
  {
    Component::LoadFrameStyle(metaFile);
    frame->SetStylingFile(metaFile);
  }

  DropDownMenu& DropDownMenu::SetDropDownItems(const std::vector<std::string>& items)
  {
    if(selectedIndex >= items.size())
      selectedIndex = 0;
    this->items = items;
    frame->SetDropDownItems(items);
    return *this;
  }

  const std::vector<std::string>& DropDownMenu::GetDropDownItems() const
  {
    return items;
  }

  void DropDownMenu::SetOnSelectionChangeCallback(OnSelectionChanged callback)
  {
    selectionChangedCallback = callback;
    selectionChangedCallback(this, "", items[selectedIndex]);
  }

  void DropDownMenu::CallOnSelectionChanged(int index)
  {
    if(selectionChangedCallback)
      selectionChangedCallback(this, items[selectedIndex], items[index]);
    selectedIndex = index;
  }

  float DropDownMenu::GetWrapWidth() const
  {
    int i = 0;
    uint32_t width = 0;
    for(int i = 0; i < items.size(); i++)
    {
      width = std::max(width, text.font.GetWidthOfText(items[i]));
    }
    return width;
  }

  float DropDownMenu::GetWrapHeight() const
  {
    return text.font.GetSize();
  }
}
