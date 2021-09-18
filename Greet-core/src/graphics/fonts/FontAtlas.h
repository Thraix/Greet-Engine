#pragma once

#include <common/Memory.h>
#include <graphics/textures/Texture2D.h>
#include <math/Vec4.h>

#include <map>

typedef struct FT_LibraryRec_  *FT_Library;
typedef struct FT_FaceRec_*  FT_Face;

namespace Greet
{
  struct Glyph
  {
    float miWidth;
    float miBearingX;
    float miAdvanceX;
    float miAdvanceY;
    float miHeight;
    float miBearingY;
    float miDescending;
    Vec4f mvTextureCoords;
  };

  class FontAtlas final
  {
    private:
      FT_Library mLibrary;
      FT_Face mFace;
      Ref<Texture2D> mAtlas;
      uint32_t miWidth;
      uint32_t miHeight;
      std::map<char, Glyph> mvGlyphs;
      ImageData mImageData;

      uint32_t miYPos;
      uint32_t miXPos;
      uint32_t miNextYPos;

      uint32_t miBaselineOffset;
      uint32_t miMedianOffset;
      uint32_t miMedianHeight;

    private:
      FontAtlas(const std::string& asFilename, uint32_t aiFontSize);

    public:
      ~FontAtlas();

      FontAtlas(const FontAtlas&) = delete;
      FontAtlas& operator=(const FontAtlas&) = delete;

      const Glyph& GetGlyph(char acCharacter);
      uint32_t GetTextureId() const;
      uint32_t GetBaselineOffset() const;
      uint32_t GetMedianOffset() const;
      uint32_t GetMedianHeight() const;

      static Ref<FontAtlas> Create(const std::string& asFontname, uint32_t aiFontSize);
    private:
      const Glyph& AddGlyph(char character);
  };
}
