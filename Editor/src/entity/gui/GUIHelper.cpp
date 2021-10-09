#include "GUIHelper.h"

#include <iomanip>

std::string GUIHelper::FloatToString(float f)
{
  std::stringstream ss;
  ss << std::setprecision(3) << f;
  return ss.str();
}

float GUIHelper::StringToFloat(const std::string& str)
{
  std::stringstream ss{str};
  float f = 0.0f;
  ss >> f;
  return f;
}
