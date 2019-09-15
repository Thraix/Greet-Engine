#include "Utils.h"

#include <internal/GreetGL.h>

namespace Greet {
  void Utils::Screenshot(uint width, uint height)
  {
    byte* pixels = new byte[4 * width * height];
    glReadPixels(0,0,width,height, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
    FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, width, height, 4 * width, 32, 0x0000ff, 0x00ff00, 0xFF0000, false);
    std::string s = "screenshot_";
    s += Utils::CurrentDateTime();
    s += ".png";
    FreeImage_Save(FIF_PNG, image, s.c_str(), 0);
    FreeImage_Unload(image);
    delete[] pixels;
  }
}
