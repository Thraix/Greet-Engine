#pragma once

#include <utils/ImageData.h>

#include <string>
#include <vector>

namespace Greet {

  struct ImageUtils
  {
    static void PrintImage(const ImageData& data);
    static ImageData FlipImage(const ImageData& bits);
    static ImageDataResult LoadImage(const std::string& filepath);
    static ImageData CropImage(const ImageData& bits, uint32_t cx,  uint32_t cy,  uint32_t cwidth, uint32_t cheight);
    static void SaveImageBytes(const std::string& filepath, const std::string& output);
    static ImageData CreateHeightmapImage(const std::vector<float>& heightMap, uint32_t width, uint32_t height);
  };
}
