#include "Utils.h"

#include <internal/GreetGL.h>
#include <math/Vec2.h>

#include <stdint.h>

namespace Greet {
  void Utils::Screenshot(uint32_t width, uint32_t height, std::string filename)
  {
    uint8_t* pixels = new uint8_t[4 * width * height];
    glReadPixels(0,0,width,height, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
    FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, width, height, 4 * width, 32, 0x0000ff, 0x00ff00, 0xFF0000, false);
    if(filename == "")
    {
      filename  = "screenshot_";
      filename += Utils::CurrentDateTime();
      filename += ".png";
    }
    FreeImage_Save(FIF_PNG, image, filename.c_str(), 0);
    FreeImage_Unload(image);
    delete[] pixels;
  }

  bool Utils::IsInside(const Vec2f& point, const Vec2f& pos, const Vec2f& size)
  {
    return point.x >= pos.x && point.x < pos.x + size.w && point.y >= pos.y && point.y < pos.y + size.h;
  }
}
