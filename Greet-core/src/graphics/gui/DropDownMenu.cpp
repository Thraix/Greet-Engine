#include "DropDownMenu.h"

#include <algorithm>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(DropDownMenu);
  DropDownMenu::DropDownMenu(const std::string& name, Component* parent)
    : DropDownMenu{name, parent, {"No items"}, "", 20}
  {
    AddStyleVariables(StylingVariables{
        .colors={
          {"textColor", &text.color},
          {"dropBackgroundColor", &dropBackgroundColor},
          {"dropBorderColor", &dropBorderColor}},
        .tlbrs={
          {"dropPadding", &dropPadding},
          {"dropBorder", &dropBorder}},
        .fonts={{"font", &text.font}}
        });
  }

  DropDownMenu::DropDownMenu(const std::string& name, Component* parent, const std::vector<std::string>& items, const std::string& fontName, float fontSize)
    : Component{name, parent}, items{items}
  {
    if(items.empty())
      this->items.emplace_back("No items");

    text.font = FontManager::Get(fontName, fontSize);
    AddStyleVariables(StylingVariables{
        .colors={
          {"textColor", &text.color},
          {"dropBackgroundColor", &dropBackgroundColor},
          {"dropBorderColor", &dropBorderColor}},
        .tlbrs={
          {"dropPadding", &dropPadding},
          {"dropBorder", &dropBorder}},
        .fonts={{"font", &text.font}}
      });
  }

  DropDownMenu::DropDownMenu(const XMLObject& object, Component* parent)
    : Component(object, parent)
  {
    AddStyleVariables(StylingVariables{
        .colors={
          {"textColor", &text.color},
          {"dropBackgroundColor", &dropBackgroundColor},
          {"dropBorderColor", &dropBorderColor}},
        .tlbrs={
          {"dropPadding", &dropPadding},
          {"dropBorder", &dropBorder}},
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
  }

  void DropDownMenu::Render(GUIRenderer* renderer) const
  {
    Text txt = text;
    txt.str = items[selectedIndex];
    renderer->PushTranslation(pos + GetTotalPadding());
    txt.Render(renderer, GetContentSize());
    renderer->PopTranslation();
    float size = GetFontSize() / 2;
    Vec2f center = pos + GetSize() - Vec2f{GetHeight(), GetHeight()} / 2.0f;
    if(!opened)
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

    if(opened)
    {
      for(int i = 0; i < items.size(); i++)
      {
        renderer->PushTranslation(pos + Vec2f{0, (float)(GetHeight() * (i + 1))});
        renderer->DrawRect({}, GetSize(), dropBorderColor, false);
        renderer->DrawRect(dropBorder.LeftTop(), GetSize() - dropBorder.GetSize(), dropBackgroundColor, false);
        txt.str = items[i];
        txt.Render(renderer, dropPadding.LeftTop() + dropBorder.LeftTop(), GetSize() - dropPadding.GetSize() - dropBorder.GetSize());
        renderer->PopTranslation();
      }
    }
  }

  DropDownMenu& DropDownMenu::SetDropDownItems(const std::vector<std::string>& items)
  {
    this->items = items;
    return *this;
  }

  const std::vector<std::string>& DropDownMenu::GetDropDownItems() const
  {
    return items;
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

  const Color& DropDownMenu::GetColor() const
  {
    return text.color;
  }

  const Font& DropDownMenu::GetFont() const
  {
    return text.font;
  }

  float DropDownMenu::GetFontSize() const
  {
    return text.font.GetSize();
  }


  DropDownMenu& DropDownMenu::SetGravity(Text::Gravity grav)
  {
    text.gravity = grav;
    return *this;
  }

  DropDownMenu& DropDownMenu::SetAlign(Text::Align align)
  {
    text.align = align;
    return *this;
  }

  DropDownMenu& DropDownMenu::SetFont(const std::string& font)
  {
    text.SetFont(font);
    return *this;
  }

  DropDownMenu& DropDownMenu::SetFontSize(float fontSize)
  {
    text.SetFontSize(fontSize);
    Remeasure();
    return *this;
  }

  DropDownMenu& DropDownMenu::SetColor(const Color& color)
  {
    text.color = color;
    return *this;
  }
}
