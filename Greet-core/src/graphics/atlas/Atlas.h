#pragma once

#include <graphics/textures/Texture2D.h>
#include <math/Vec2.h>
#include <utils/ImageData.h>

#include <map>
#include <vector>

namespace Greet{
  struct AtlasCoords
  {
    Vec2f coord1;
    Vec2f coord2;
  };

  class Atlas
  {
    private:
      Ref<Texture2D> texture;

      std::map<std::string, uint32_t> textureMap;
      std::vector<bool> occupied;

      uint32_t textureSize;
      uint32_t textureCountSide;
      uint32_t textureCountTotal;

      uint8_t* bits;
    public:
      Atlas(uint32_t atlasSize, uint32_t textureSize);
      virtual ~Atlas();

      bool AddTexture(const std::string& name, const std::string& filePath);
      void RemoveTexture(const std::string& textureName);
      bool AddTexture(const ImageData& bits, const std::string& name);
      AtlasCoords GetTextureCoords(const std::string& sheetName) const;
      int GetTextureSize() const { return textureSize; }
      int GetAtlasSize() const { return texture->GetWidth(); }

      void Enable(int index = 0) const;
      void Disable() const;

      Ref<Texture2D> GetAtlasTexture() const { return texture; }
  };
}
