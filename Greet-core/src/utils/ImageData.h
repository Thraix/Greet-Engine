#pragma once

#include <common/Memory.h>

#include <stdint.h>

#define IMAGE_DATA_RED   0
#define IMAGE_DATA_GREEN 1
#define IMAGE_DATA_BLUE  2
#define IMAGE_DATA_ALPHA 3

namespace Greet
{
  struct ImageData
  {
    Ref<uint8_t[]> data; // Format=RGBA
    uint32_t width;
    uint32_t height;

    ImageData(uint32_t width, uint32_t height)
      : data{new uint8_t[width * height * 4]}, width{width}, height{height}
    {}

    uint8_t& at(uint32_t x, uint32_t y, uint8_t channel)
    {
      return data[(x + y * width) * 4 + channel];
    }

    const uint8_t& at(uint32_t x, uint32_t y, uint8_t channel) const
    {
      return data[(x + y * width) * 4 + channel];
    }

    uint32_t& at(uint32_t x, uint32_t y)
    {
      return ((uint32_t*)data.get())[x + y * width];
    }

    const uint32_t& at(uint32_t x, uint32_t y) const
    {
      return ((uint32_t*)data.get())[x + y * width];
    }
  };

  using ImageDataResult = std::pair<bool, ImageData>; // <load successful, data>
}
