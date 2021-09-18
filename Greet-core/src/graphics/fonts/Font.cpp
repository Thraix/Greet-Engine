#include "Font.h"
#include <logging/Log.h>

#include <graphics/fonts/FontContainer.h>

namespace Greet{

  Font::Font(const Ref<FontAtlas>& aAtlas, FontContainer* aContainer, uint32_t aiSize)
    : mAtlas(aAtlas), mContainer{aContainer}, mfSize(aiSize)
  {}

  const Ref<FontAtlas>& Font::GetFontAtlas() const
  {
    return mAtlas;
  }

  uint32_t Font::GetFontAtlasId() const
  {
    return mAtlas->GetTextureId();
  }

  const std::string& Font::GetName() const
  {
    return mContainer->GetName();
  }

  uint32_t Font::GetSize() const
  {
    return mfSize;
  }

  uint32_t Font::GetBaselineOffset() const
  {
    return mAtlas->GetBaselineOffset();
  }

  uint32_t Font::GetMedianOffset() const
  {
    return mAtlas->GetMedianOffset();
  }

  uint32_t Font::GetMedianHeight() const
  {
    return mAtlas->GetMedianHeight();
  }

  uint32_t Font::GetWidthOfText(const std::string_view& asText) const
  {
    uint32_t width = 0;
    for (uint32_t i = 0;i < asText.size();i++)
    {
      const Glyph& glyph = mAtlas->GetGlyph(asText[i]);
      // If it is the last char do not include the advancement
      if(i == asText.size() - 1 && asText[i] != ' ')
        width += glyph.miWidth;
      else
        width += glyph.miAdvanceX;
    }
    return width;
  }

  uint32_t Font::GetWidthOfText(const std::string& asText, uint32_t aiStartPos, uint32_t aiEndPos) const
  {
    if (aiStartPos > asText.size() || aiEndPos < aiStartPos || aiEndPos > asText.size())
    {
      Log::Error("Invalid start and endpos (start=%s, end=%s strlen=%s)", aiStartPos, aiEndPos, asText.size());
      return 0;
    }
    return GetWidthOfText(std::string_view(asText.c_str() + aiStartPos, aiEndPos - aiStartPos));
  }

  uint32_t Font::GetWidthOfText(const std::string& asText) const
  {
    return GetWidthOfText(asText, 0, asText.size());
  }

  std::vector<uint32_t> Font::GetPartialWidths(const std::string& asText) const
  {
    float width = 0;

    std::vector<uint32_t> widths;

    for (uint32_t i = 0;i < asText.size();i++)
    {
      const Glyph& glyph = mAtlas->GetGlyph(asText[i]);
      widths.push_back(width);
      // If it is the last char do not include the advancment
      if(i == asText.size()- 1 && asText[i] != ' ')
        width += glyph.miWidth;
      else
        width += glyph.miAdvanceX;
    }
    widths.push_back(width);
    return widths;
  }
}
