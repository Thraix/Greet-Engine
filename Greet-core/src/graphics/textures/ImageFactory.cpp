#include "ImageFactory.h"

namespace Greet {
  ImageData ImageFactory::GetBadFormatImage()
  {
    return GetErrorImage(0xff00ff, 0xaa00aa);
  }

  ImageData ImageFactory::GetBadBPPImage()
  {
    return GetErrorImage(0xffff00, 0xaaaa00);
  }

  ImageData ImageFactory::GetCantReadImage()
  {
    return GetErrorImage(0x00ffff, 0x00aaaa);
  }

  ImageData ImageFactory::GetCropErrorImage()
  {
    return GetErrorImage(0xff0000, 0xaa0000);
  }

  ImageData ImageFactory::GetErrorImage(uint32_t lightColor, uint32_t darkColor)
  {
    ImageData imageData(16, 16);
    for(uint32_t y = 0;y < imageData.height; y++)
    {
      for(uint32_t x = 0; x < imageData.width; x++)
      {
        uint32_t color = lightColor;
        if(x == 0 || y == imageData.height - 1)
          color = darkColor;
        imageData.at(x, y, IMAGE_DATA_RED) = (color & 0xff0000) >> 16;
        imageData.at(x, y, IMAGE_DATA_GREEN) = (color & 0xff00) >> 8;
        imageData.at(x, y, IMAGE_DATA_BLUE) = color & 0xff;
        imageData.at(x, y, IMAGE_DATA_ALPHA) = 0xff;
      }
    }
    return imageData;
  }
}
