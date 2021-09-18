#pragma once

#include <graphics/fonts/Font.h>

#include <string>
#include <unordered_map>

namespace Greet{

  class FontContainer
  {
    public:
      friend class Font;
      friend class FontManager;

    private:
      std::string msFilename;
      std::string msName;
      std::unordered_map<uint32_t, Font> mvFonts;

    public:
      FontContainer(const std::string& asFilename);

      Font GetFont(uint32_t aiSize);
      const std::string& GetFilename() const { return msFilename; }
      const std::string& GetName() const { return msName; }

    private:
      void SetName(const std::string& asName) { msName = asName; }

  };
}
