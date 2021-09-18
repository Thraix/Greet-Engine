#include "CubeMap.h"

#include <internal/GreetGL.h>
#include <utils/ImageUtils.h>

namespace Greet {
  CubeMap::CubeMap(const std::string& top, const std::string& bottom, const std::string& left, const std::string& right, const std::string& front, const std::string& back)
    : Texture(GL_TEXTURE_CUBE_MAP)
  {
    LoadCubeMap(top, bottom, left, right, front, back);
  }

  CubeMap::CubeMap(const std::string& map)
    : Resource(map), Texture(GL_TEXTURE_CUBE_MAP)
  {
    uint32_t width;
    uint32_t height;
    auto res = ImageUtils::LoadImage(map.c_str());
    LoadCubeMap(res.second, res.first);
  }

  CubeMap::CubeMap(const ImageData& imageData)
    : Texture(GL_TEXTURE_CUBE_MAP)
  {
    LoadCubeMap(imageData, true);
  }

  CubeMap::CubeMap(uint32_t texId)
    : Texture(texId, (uint32_t)GL_TEXTURE_CUBE_MAP)
  {

  }

  CubeMap::CubeMap()
    : Texture(GL_TEXTURE_CUBE_MAP)
  {
  }

  void CubeMap::ReloadResource()
  {
    auto res = ImageUtils::LoadImage(filePath.c_str());
    LoadCubeMap(res.second, res.first);
  }

  void CubeMap::LoadCubeMap(const ImageData& imageData, bool printDimensionError)
  {
    if(imageData.width % 4 != 0 || imageData.height % 3 != 0)
    {
      LoadImage(imageData, GL_TEXTURE_CUBE_MAP_POSITIVE_X, false);
      LoadImage(imageData, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, false);
      LoadImage(imageData, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, false);
      LoadImage(imageData, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, false);
      LoadImage(imageData, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, false);
      LoadImage(imageData, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, false);
    }
    else
    {
      uint32_t w = imageData.width / 4;
      uint32_t h = imageData.height / 3;
      LoadImage(ImageUtils::CropImage(imageData, 0, h, w, h), GL_TEXTURE_CUBE_MAP_POSITIVE_X, true);
      LoadImage(ImageUtils::CropImage(imageData, w*2, h, w, h), GL_TEXTURE_CUBE_MAP_NEGATIVE_X, true);

      LoadImage(ImageUtils::CropImage(imageData, w, 0, w, h), GL_TEXTURE_CUBE_MAP_POSITIVE_Y, true);
      LoadImage(ImageUtils::CropImage(imageData, w, h*2, w, h), GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, true);

      LoadImage(ImageUtils::CropImage(imageData, w, h, w, h), GL_TEXTURE_CUBE_MAP_POSITIVE_Z, true);
      LoadImage(ImageUtils::CropImage(imageData, w*3, h, w, h), GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, true);
    }
    LoadParameters();

    Disable();
  }

  void CubeMap::LoadCubeMap(const std::string& top, const std::string& bottom, const std::string& left, const std::string& right, const std::string& front, const std::string& back)
  {
    Enable(0);

    LoadImage(right, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    LoadImage(left, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
    LoadImage(top, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
    LoadImage(bottom, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
    LoadImage(back, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
    LoadImage(front, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

    LoadParameters();

    Disable();
  }

  void CubeMap::LoadParameters()
  {
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
  }

  void CubeMap::LoadImage(const std::string& image, uint32_t mapLocation)
  {
    uint32_t width;
    uint32_t height;
    auto res = ImageUtils::LoadImage(image.c_str());
    LoadImage(res.second, mapLocation, true);
  }

  void CubeMap::LoadImage(const ImageData& imageData, uint32_t mapLocation, bool flip)
  {
    if(flip)
    {
      GLCall(glTexImage2D(mapLocation, 0, GL_RGBA, imageData.width, imageData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImageUtils::FlipImage(imageData).data.get()));
    }
    else
    {
      GLCall(glTexImage2D(mapLocation, 0, GL_RGBA, imageData.width, imageData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData.data.get()));
    }
  }
}
