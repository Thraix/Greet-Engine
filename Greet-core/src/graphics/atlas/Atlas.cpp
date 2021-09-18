#include "Atlas.h"

#include <internal/GreetGL.h>
#include <utils/ImageUtils.h>

namespace Greet {

  Atlas::Atlas(uint32_t atlasSize, uint32_t textureSize)
    : texture(Texture2D::Create(atlasSize, atlasSize, TextureParams(TextureFilter::NEAREST,TextureWrap::NONE,TextureInternalFormat::RGBA))), textureSize(textureSize)
  {
    ASSERT(atlasSize > textureSize, "Atlas size must be greater than the textures sizes");
    //ASSERT(!(atlasSize == 0) && !(atlasSize & (atlasSize - 1)),"ATLAS", "Atlas size must be a power of two");
    //ASSERT(!(textureSize == 0) && !(textureSize & (textureSize - 1)), "ATLAS", "Texture size must be a power of two");
    textureCountSide = atlasSize / textureSize;
    textureCountTotal = textureCountSide * textureCountSide;
    uint32_t bitCount = atlasSize * atlasSize * 4;

    occupied.resize(textureCountTotal);
    std::fill(occupied.begin(), occupied.end(), false);

    ImageData imageData(atlasSize, atlasSize);
    for(uint32_t y = 0; y < atlasSize; y++)
    {
      for(uint32_t x = 0; x < atlasSize; x++)
      {
        imageData.at(x, y, IMAGE_DATA_RED)   = 255;
        imageData.at(x, y, IMAGE_DATA_GREEN) = 0;
        imageData.at(x, y, IMAGE_DATA_BLUE)  = 255;
        imageData.at(x, y, IMAGE_DATA_ALPHA) = 255;
      }
    }

    texture->SetPixels(imageData);
  }

  Atlas::~Atlas()
  {
  }

  bool Atlas::AddTexture(const std::string& name, const std::string& filePath)
  {
    uint32_t width;
    uint32_t height;
    auto res = ImageUtils::LoadImage(filePath.c_str());
    if (width != textureSize || height != textureSize)
    {
      Log::Error("The given textures size is not valid: %s (%s, %s)", name.c_str(), width, height);
      return false;
    }

    return AddTexture(res.second, name);
  }

  bool Atlas::AddTexture(const ImageData& bits, const std::string& name)
  {
    if (textureMap.size() >= textureCountTotal)
    {
      Log::Error("There is no more room in the Atlas. Increase size or create a new one.");
      return false;
    }
    uint32_t x = textureCountSide;
    uint32_t y = textureCountSide;

    for (uint32_t i = 0; i < textureCountTotal; i++)
    {
      if (!occupied[i])
      {
        textureMap.emplace(name,i);
        occupied[i] = true;
        x = i % textureCountSide;
        y = (i - x) / textureCountSide;
        GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, x*textureSize,textureSize*textureCountSide -textureSize - y*textureSize,textureSize,textureSize,GL_RGBA, GL_UNSIGNED_BYTE, bits.data.get()));
        return true;
      }
    }
    ASSERT(false, "Atlas failed to find unoccupied space, even though it passed a space check");
  }

  AtlasCoords Atlas::GetTextureCoords(const std::string& sheetName) const
  {
    auto it = textureMap.find(sheetName);
    if(it != textureMap.end())
    {
      uint32_t j = it->second;
      uint32_t x = j % textureCountSide;
      uint32_t y = (j - x) / textureCountSide;

      float size = textureSize / (float)texture->GetWidth();
      Vec2f coord1 = Vec2f(size, size) * Vec2f(x, y);
      Vec2f coord2 = coord1 + Vec2f(size, size);
      return AtlasCoords{coord1, coord2};
    }

    Log::Error("No texture found in Atlas: (%s)", sheetName.c_str());
    return AtlasCoords{{0,0}, {1,1}};
  }

  void Atlas::RemoveTexture(const std::string& textureName)
  {
    auto it = textureMap.find(textureName);
    if(it != textureMap.end())
    {
      occupied[it->second] = false;
      textureMap.erase(it);
    }
  }

  void Atlas::Enable(int index) const
  {
    texture->Enable(index);
  }

  void Atlas::Disable() const
  {
    texture->Disable();
  }
}
