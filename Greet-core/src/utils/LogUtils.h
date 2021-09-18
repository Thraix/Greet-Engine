#pragma once

#include <logging/Log.h>

// TODO: Move implementation to cpp file

namespace Greet {

  struct LogUtils
  {
    static uint32_t Hex10ToDec16(const char c)
    {
      if (c >= '0' && c <= '9')
        return (uint32_t)(c - '0');
      else if (c >= 'a' && c <= 'f')
        return (uint32_t)(c - 'a' + 10);
      else if(c >= 'A' && c <= 'F')
        return (uint32_t)(c - 'A' + 10);
      Log::Warning("Utils::Hex10ToDec16 couldn't recognize the hex character \'%s\'", c);
      return 0;
    }

    static char Dec16ToHex10(int dec)
    {
      switch (dec)
      {
        case 0:
          return '0';
        case 1:
          return '1';
        case 2:
          return '2';
        case 3:
          return '3';
        case 4:
          return '4';
        case 5:
          return '5';
        case 6:
          return '6';
        case 7:
          return '7';
        case 8:
          return '8';
        case 9:
          return '9';
        case 10:
          return 'a';
        case 11:
          return 'b';
        case 12:
          return 'c';
        case 13:
          return 'd';
        case 14:
          return 'e';
        case 15:
          return 'f';
        default:
          return '0';
      }
    }

    static std::string DecToHex(uint32_t dec, uint32_t values = 1)
    {
      std::string s = "";
      bool trailing = true;
      for (uint32_t i = 8; i >= 1; i--)
      {
        char hex = Dec16ToHex10(((dec >> (i * 4 - 4)) & 0xf));
        if (hex != '0' || i <= values || !trailing)
        {
          s += hex;
          if (hex != '0')
            trailing = false;
        }
      }
      return s;
    }

    static uint32_t HexToDec(const std::string& hex)
    {
      if (hex.length() > 8)
      {
        return 0;
      }
      else
      {
        uint32_t result = 0;

        int size = hex.length();
        for (int i = 0; i < size; i++)
        {
          result = (result << 4) | Hex10ToDec16(hex[i]);
        }
        return result;
      }
    }
  };
}
