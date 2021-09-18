#include "FontContainer.h"

namespace Greet{

  FontContainer::FontContainer(const std::string& asFilename)
    : msFilename(asFilename)
  {
  }

  Font FontContainer::GetFont(uint32_t aiSize)
  {
    // TODO: Automatically remove the fonts if it's not used anymore
    // (check if Ref<FontAtlas> has count 1)
    auto it = mvFonts.find(aiSize);
    if (it == mvFonts.end())
    {
      Font font(FontAtlas::Create(msFilename, aiSize), this, aiSize);
      mvFonts.emplace(aiSize, font);
      return font;
    }
    return it->second;
  }
}
