#include "Text.h"

namespace Greet
{
  Text::Text(const std::string& str, const Font& font, const Color& color, Gravity gravity, Align align, OverlapMode overlapMode)
    : str{str}, font{font}, color{color}, gravity{gravity}, align{align}, overlapMode{overlapMode}
  {
overlapMode = OverlapMode::Wrap;
  }

  void Text::Render(GUIRenderer* renderer, const Vec2f& contentSize) const
  {
    std::vector<std::string> lines = GetStringLines(contentSize.w);
    Vec2f offset{};

    if(gravity == Gravity::Top)
      offset.y = font.GetBaselineOffset();
    else if(gravity == Gravity::Center)
    {
      offset.y = contentSize.h;
      if(lines.size() % 2 == 0)
        offset.y -= lines.size() * font.GetSize();
      else
        offset.y -= (lines.size() - 1) * font.GetSize() + font.GetBaselineOffset();
      offset.y /= 2.0f;
      offset.y +=  font.GetBaselineOffset();
    }
    else
      offset.y = contentSize.h - lines.size() * font.GetSize() + font.GetBaselineOffset();

    for(auto&& line : lines)
    {
      uint32_t widthOfText = font.GetWidthOfText(line);
      if(align == Align::Left)
        offset.x = 0;
      else if(align == Align::Center)
        offset.x = (contentSize.w - widthOfText) / 2;
      else
        offset.x = contentSize.w - widthOfText;

      renderer->DrawText(std::string(line), offset, font, color, false);

      offset.y += font.GetSize();
    }
  }


  std::vector<std::string> Text::GetStringLines(float maxWidth) const
  {
    if(overlapMode == OverlapMode::Wrap)
    {
      std::vector<std::string> lines;
      size_t begin = 0;
      size_t end = str.find(' ');
      size_t previousEnd = end;
      while(end != std::string::npos)
      {
        uint32_t width = font.GetWidthOfText(str, begin, end);
        if(width > maxWidth)
        {
          lines.push_back(str.substr(begin, previousEnd - begin));
          begin = previousEnd + 1;
        }
        previousEnd = end;
        end = str.find(' ', end + 1);
      }

      uint32_t width = font.GetWidthOfText(str, begin, str.size());
      if(width > maxWidth)
      {
        lines.push_back(str.substr(begin, previousEnd - begin));
        begin = previousEnd + 1;
      }
      lines.push_back(str.substr(begin, str.size() - begin));
      return lines;
    }
    else
    {
      std::string text = str;
      uint32_t widthOfText = font.GetWidthOfText(text);
      if(overlapMode == OverlapMode::Dots)
      {
        if(widthOfText > maxWidth)
          text += "...";
        while(widthOfText > maxWidth && text.size() > 3)
        {
          text.erase(text.begin() + text.size() - 4, text.end());
          text += "...";
          widthOfText = font.GetWidthOfText(text);
        }
      }
      return {text};
    }
  }

  float Text::GetWrapWidth() const
  {
    return font.GetWidthOfText(str);
  }

  float Text::GetWrapHeight(float maxWidth) const
  {
    std::vector<std::string> lines = GetStringLines(maxWidth);
    return (float)lines.size() * font.GetSize();
  }
}
