#pragma once

#include <common/Memory.h>
#include <graphics/fonts/FontAtlas.h>

#include <string>

namespace Greet{

  class FontContainer;

  class Font final
  {
    private:
      Ref<FontAtlas> mAtlas;
      FontContainer* mContainer;
      uint32_t mfSize;

    public:
      Font(const Ref<FontAtlas>& aAtlas, FontContainer* aContainer, uint32_t aiSize);

      const Ref<FontAtlas>& GetFontAtlas() const;
      uint32_t GetFontAtlasId() const;


      const std::string& GetName() const;

      uint32_t GetSize() const;
      uint32_t GetBaselineOffset() const;
      uint32_t GetMedianOffset() const;
      uint32_t GetMedianHeight() const;

      uint32_t GetWidthOfText(const std::string& asText, uint32_t startPos, uint32_t endPos) const;
      uint32_t GetWidthOfText(const std::string& asText) const;
      uint32_t GetWidthOfText(const std::string_view& asText) const;

      std::vector<uint32_t> GetPartialWidths(const std::string& asText) const;
  };
}
