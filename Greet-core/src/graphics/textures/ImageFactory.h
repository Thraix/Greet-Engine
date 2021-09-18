#pragma once

#include <utils/ImageData.h>

namespace Greet {

  struct ImageFactory
  {
    static ImageData GetBadFormatImage();
    static ImageData GetBadBPPImage();
    static ImageData GetCantReadImage();
    static ImageData GetCropErrorImage();
    static ImageData GetErrorImage(uint32_t lightColor, uint32_t darkColor);
    static ImageData GetFrameImage();
  };
}
