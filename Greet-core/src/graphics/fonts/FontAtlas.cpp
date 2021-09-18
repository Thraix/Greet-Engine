#include "FontAtlas.h"

#include <internal/GreetGL.h>

namespace Greet
{
  FontAtlas::FontAtlas(const std::string& asFilename, uint32_t aiFontSize)
    : miYPos{0}, miXPos{0}, miNextYPos{0}, miWidth{512}, miHeight{512}, mAtlas{Texture2D::Create(512, 512, TextureParams(TextureFilter::NEAREST, TextureWrap::CLAMP_TO_EDGE, TextureInternalFormat::RGBA))}, mImageData{miWidth, miHeight}
  {
    if(FT_Init_FreeType(&mLibrary))
    {
      Log::Error("Could not initialize FreeType.");
    }
    if(FT_New_Face(mLibrary, asFilename.c_str(), 0, &mFace))
    {
      Log::Error("Could not initialize font: ", asFilename);
    }

    FT_Set_Pixel_Sizes(mFace, 0, aiFontSize);

    miBaselineOffset = (mFace->size->metrics.ascender + mFace->size->metrics.descender) / 64;

    const Glyph& g = GetGlyph('o'); // This character usually has median bearing and height
    miMedianOffset = miBaselineOffset - g.miBearingY;
    miMedianHeight = g.miHeight;
  }

  FontAtlas::~FontAtlas()
  {
    FT_Done_Face(mFace);
    FT_Done_FreeType(mLibrary);
  }

  const Glyph& FontAtlas::GetGlyph(char acCharacter)
  {
    auto it = mvGlyphs.find(acCharacter);
    if(it != mvGlyphs.end())
    {
      return it->second;
    }
    return AddGlyph(acCharacter);
  }

  const Glyph& FontAtlas::AddGlyph(char acCharacter)
  {
    if(FT_Load_Char(mFace,acCharacter,FT_LOAD_RENDER))
    {
      Log::Error("Could not load character: ", acCharacter);
      // Lets just hope that there is a character in the map. Maybe add a dummy character later
      return mvGlyphs.begin()->second;
    }

    uint32_t pixelWidth = mFace->glyph->bitmap.width;
    uint32_t pixelHeight = mFace->glyph->bitmap.rows;
    if(miXPos + pixelWidth >= miWidth)
    {
      miXPos = 0;
      miYPos = miNextYPos + 1;
    }
    // Set new y value.
    if(miYPos+pixelHeight > miNextYPos)
      miNextYPos = miYPos+pixelHeight;
    for(uint32_t y = 0;y<pixelHeight;y++)
    {
      for(uint32_t x = 0;x<pixelWidth;x++)
      {
        unsigned char a = mFace->glyph->bitmap.buffer[(x+y*pixelWidth)];
        mImageData.at(x + miXPos, y + miYPos, IMAGE_DATA_RED) = 0xff;
        mImageData.at(x + miXPos, y + miYPos, IMAGE_DATA_GREEN) = 0xff;
        mImageData.at(x + miXPos, y + miYPos, IMAGE_DATA_BLUE) = 0xff;
        mImageData.at(x + miXPos, y + miYPos, IMAGE_DATA_ALPHA) = a;
      }
    }
    Glyph g;
    g.miWidth = mFace->glyph->metrics.width / 64;
    g.miBearingX = mFace->glyph->metrics.horiBearingX / 64;
    g.miAdvanceX = mFace->glyph->metrics.horiAdvance / 64;
    g.miAdvanceY = mFace->glyph->metrics.vertAdvance / 64;
    g.miHeight = mFace->glyph->metrics.height / 64;
    g.miBearingY = (mFace->glyph->metrics.horiBearingY) / 64;
    g.miDescending = (g.miHeight - mFace->glyph->metrics.horiBearingY) / 64;
    g.mvTextureCoords = Vec4(miYPos / (float)miHeight, miXPos / (float)miWidth, (miYPos+pixelHeight)/(float)miHeight, (miXPos+pixelWidth)/(float)miWidth);
    miXPos += pixelWidth + 1;
    std::pair<std::map<char,  Glyph>::iterator, bool> ret = mvGlyphs.emplace(acCharacter, g);
    mAtlas->SetPixels(mImageData);
    return ret.first->second;
  }

  uint32_t FontAtlas::GetTextureId() const
  {
    return mAtlas->GetTexId();
  }

  uint32_t FontAtlas::GetBaselineOffset() const
  {
    return miBaselineOffset;
  }

  uint32_t FontAtlas::GetMedianOffset() const
  {
    return miMedianOffset;
  }

  uint32_t FontAtlas::GetMedianHeight() const
  {
    return miMedianHeight;
  }

  Ref<FontAtlas> FontAtlas::Create(const std::string& asFontname, uint32_t aiFontSize)
  {
    return Ref<FontAtlas>{new FontAtlas(asFontname, aiFontSize)};
  }
}
