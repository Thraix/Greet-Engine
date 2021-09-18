#include "FontManager.h"

#include <logging/Log.h>

#include <stdint.h>

namespace Greet{

  std::unordered_map<std::string, FontContainer> FontManager::mvFonts;

  bool FontManager::Add(const std::string& asFontname, const FontContainer& aFont)
  {
    auto it = mvFonts.emplace(asFontname, aFont);
    if(!it.second)
    {
      Log::Error("Given font name already exists: ", asFontname);
      return false;
    }
    it.first->second.SetName(asFontname);
    return true;
  }

  Font FontManager::Get(const std::string& asFontname, uint32_t aiSize)
  {
    auto it = mvFonts.find(asFontname);
    if(it == mvFonts.end())
    {
      Log::Error("Could not find the given font: ", asFontname);
      return mvFonts.begin()->second.GetFont(aiSize);
    }
    return it->second.GetFont(aiSize);
  }

  // TODO: REMOVE FONTS

  void FontManager::Destroy()
  {
    mvFonts.clear();
  }

}
