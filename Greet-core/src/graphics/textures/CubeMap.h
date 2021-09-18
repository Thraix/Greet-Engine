#pragma once

#include <graphics/textures/Texture.h>
#include <utils/ImageData.h>
#include <utils/Resource.h>

#include <string>

namespace Greet {

  class CubeMap : public Texture, public Resource
  {
    public:
      CubeMap(const std::string& top, const std::string& bottom, const std::string& left, const std::string& right, const std::string& front, const std::string& back);
      CubeMap(const std::string& map);
      CubeMap(const ImageData& imageData);
      CubeMap(uint32_t texId);
      CubeMap();
      void ReloadResource() override;
    private:
      void LoadCubeMap(const ImageData& imageData, bool printDimensionError);
      void LoadCubeMap(const std::string& top, const std::string& bottom, const std::string& left, const std::string& right, const std::string& front, const std::string& back);
      void LoadParameters();
      void LoadImage(const std::string& image, uint32_t mapLocation);
      void LoadImage(const ImageData& imageData, uint32_t mapLocation, bool flip);
  };
}
