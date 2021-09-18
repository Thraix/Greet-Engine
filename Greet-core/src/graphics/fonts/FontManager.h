#pragma once

#include <graphics/fonts/FontContainer.h>

#include <unordered_map>

namespace Greet {
  class FontManager
  {
    private:
      static std::unordered_map<std::string, FontContainer> mvFonts;
    public:

      static bool Add(const std::string& asFontname, const FontContainer& aFont);
      static Font Get(const std::string& asFontname, uint32_t aiSize);
      static void Destroy();
    private:
      FontManager(){}
  };
}
