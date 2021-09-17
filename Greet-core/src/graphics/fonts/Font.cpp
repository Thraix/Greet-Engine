#include "Font.h"
#include <logging/Log.h>

#include <graphics/fonts/FontContainer.h>

namespace Greet{

  Font::Font(const Ref<FontAtlas>& aAtlas, FontContainer* aContainer, uint aiSize)
    : mAtlas(aAtlas), mContainer{aContainer}, mfSize(aiSize)
  {}

  const Ref<FontAtlas>& Font::GetFontAtlas() const
  {
    return mAtlas;
  }

  uint Font::GetFontAtlasId() const
  {
    return mAtlas->GetTextureId();
  }

  const std::string& Font::GetName() const
  {
    return mContainer->GetName();
  }

  uint Font::GetSize() const
  {
    return mfSize;
  }

  uint Font::GetBaselineOffset() const
  {
    return mAtlas->GetBaselineOffset();
  }

  uint Font::GetMedianOffset() const
  {
    return mAtlas->GetMedianOffset();
  }

  uint Font::GetMedianHeight() const
  {
    return mAtlas->GetMedianHeight();
  }

  uint Font::GetWidthOfText(const std::string_view& asText) const
  {
    uint width = 0;
    for (uint i = 0;i < asText.size();i++)
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

  uint Font::GetWidthOfText(const std::string& asText, uint aiStartPos, uint aiEndPos) const
  {
    if (aiStartPos > asText.size() || aiEndPos < aiStartPos || aiEndPos > asText.size())
    {
      Log::Error("Invalid start and endpos (start=%s, end=%s strlen=%s)", aiStartPos, aiEndPos, asText.size());
      return 0;
    }
    return GetWidthOfText(std::string_view(asText.c_str() + aiStartPos, aiEndPos - aiStartPos));
  }

  uint Font::GetWidthOfText(const std::string& asText) const
  {
    return GetWidthOfText(asText, 0, asText.size());
  }

  std::vector<uint> Font::GetPartialWidths(const std::string& asText) const
  {
    float width = 0;

    std::vector<uint> widths;

    for (uint i = 0;i < asText.size();i++)
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
