#include "GUIHelper.h"

#include <iomanip>

std::string GUIHelper::FloatToString(float f)
{
  std::stringstream ss;
  ss << std::fixed << std::setprecision(3) << f;
  return ss.str();
}

float GUIHelper::StringToFloat(const std::string& str)
{
  std::stringstream ss{str};
  float f = 0.0f;
  ss >> f;
  return f;
}

std::string GUIHelper::IntToString(int i)
{
  std::stringstream ss;
  ss << i;
  return ss.str();
}

int GUIHelper::StringToInt(const std::string& str)
{
  std::stringstream ss{str};
  int i = 0;
  ss >> i;
  return i;
}
