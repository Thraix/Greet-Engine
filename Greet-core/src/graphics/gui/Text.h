#pragma once

#include <graphics/Color.h>
#include <graphics/fonts/Font.h>
#include <graphics/fonts/FontManager.h>
#include <graphics/renderers/GUIRenderer.h>

namespace Greet
{
  class Text
  {
    public:
      enum class Gravity
      {
        Top, Center, Bottom
      };

      enum class Align
      {
        Left, Center, Right
      };

      enum class OverlapMode
      {
        Dots, Wrap, Ignore
      };

      std::string str;
      Font font = FontManager::Get("noto", 12);
      Color color{0, 0, 0};
      Gravity gravity;
      Align align;
      OverlapMode overlapMode;

      Text(const std::string& str, const Font& font = FontManager::Get("noto", 18), const Color& color = {0, 0, 0}, Gravity gravity = Gravity::Center, Align align = Align::Left, OverlapMode overlapMode = OverlapMode::Dots);

      void Render(GUIRenderer* renderer, const Vec2f& contentSize) const;

      void SetFont(const std::string& fontname)
      {
        font = FontManager::Get(fontname, font.GetSize());
      }

      std::vector<std::string> GetStringLines(float maxWidth) const;

      float GetWrapWidth() const;
      float GetWrapHeight(float maxWidth) const;

      void SetFontSize(float fontSize)
      {
        font = FontManager::Get(font.GetName(), fontSize);
      }
  };
}
