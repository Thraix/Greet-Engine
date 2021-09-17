#include "FontAtlas.h"

#include <math/Vec3.h>
#include <internal/GreetGL.h>

namespace Greet
{
  FontAtlas::FontAtlas(const std::string& asFilename, uint aiFontSize)
    : miYPos{0}, miXPos{0}, miNextYPos{0}, miWidth{512}, miHeight{512}, mAtlas{Texture2D::Create(512, 512, TextureParams(TextureFilter::NEAREST, TextureWrap::CLAMP_TO_EDGE, TextureInternalFormat::RGBA))}, mvPixels(miWidth * miHeight * 4)
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

    memset(mvPixels.data(), 0, miWidth * miHeight * 4);

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

    uint pixelWidth = mFace->glyph->bitmap.width;
    uint pixelHeight = mFace->glyph->bitmap.rows;
    if(miXPos + pixelWidth >= miWidth)
    {
      miXPos = 0;
      miYPos = miNextYPos + 1;
    }
    // Set new y value.
    if(miYPos+pixelHeight > miNextYPos)
      miNextYPos = miYPos+pixelHeight;
    for(uint y = 0;y<pixelHeight;y++)
    {
      for(uint x = 0;x<pixelWidth;x++)
      {
        unsigned char a = mFace->glyph->bitmap.buffer[(x+y*pixelWidth)];
        mvPixels[4*((x+miXPos) + (y+miYPos) * miWidth)+0] = 0xff;
        mvPixels[4*((x+miXPos) + (y+miYPos) * miWidth)+1] = 0xff;
        mvPixels[4*((x+miXPos) + (y+miYPos) * miWidth)+2] = 0xff;
        mvPixels[4*((x+miXPos) + (y+miYPos) * miWidth)+3] = a;
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
    mAtlas->SetPixels(mvPixels);
    return ret.first->second;
  }

  uint FontAtlas::GetTextureId() const
  {
    return mAtlas->GetTexId();
  }

  uint FontAtlas::GetBaselineOffset() const
  {
    return miBaselineOffset;
  }

  uint FontAtlas::GetMedianOffset() const
  {
    return miMedianOffset;
  }

  uint FontAtlas::GetMedianHeight() const
  {
    return miMedianHeight;
  }

  Ref<FontAtlas> FontAtlas::Create(const std::string& asFontname, uint aiFontSize)
  {
    return Ref<FontAtlas>{new FontAtlas(asFontname, aiFontSize)};
  }
}
