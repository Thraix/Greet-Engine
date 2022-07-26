#pragma once

#include <string>

namespace Greet
{
  class GUIHelper
  {
    public:
      static std::string FloatToString(float f);
      static float StringToFloat(const std::string& str);

      static std::string IntToString(int f);
      static int StringToInt(const std::string& str);
  };
}
