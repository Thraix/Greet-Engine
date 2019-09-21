#pragma once

#include <utils/ImageUtils.h>
#include <graphics/textures/Texture.h>
#include <graphics/Sprite.h>
#include <list>
#include <vector>

namespace Greet{
  class Atlas : public Texture2D
  {

    private:
      std::map<std::string, uint> textureMap;
      std::vector<bool> occupied;

      uint m_textureSize;
      uint m_texturesSide;
      uint m_textures;

      byte* m_bits;
    public:
      Atlas(uint atlasSize, uint textureSize);
      Atlas(Atlas&&) = default;
      Atlas& operator=(Atlas&&) = default;
      virtual ~Atlas();

      bool AddTexture(const std::string& name, const std::string& filePath);
      void RemoveTexture(const std::string& textureName);
      bool AddTexture(const std::vector<byte>& bits, const std::string& name);
      Sprite GetSprite(const std::string& name) const;
      Sprite GetSpriteFromSheet(const std::string& sheetName, Vec2 texPos, Vec2 texSize) const;
      int GetTextureSize() const { return m_textureSize; }
      int GetAtlasSize() const { return GetWidth(); }
    private:
      void GenTexture(byte* bits);
  };
}
