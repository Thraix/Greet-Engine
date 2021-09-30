#pragma once

#include <logging/Log.h>
#include <utils/StringUtils.h>

#include <stdint.h>
#include <string>

namespace Greet {

  struct LogUtils
  {
    static uint32_t Hex10ToDec16(const char c);
    static char Dec16ToHex10(int dec);
    static std::string DecToHex(uint32_t dec, uint32_t values = 1);
    static uint32_t HexToDec(const std::string& hex);
  };
}
