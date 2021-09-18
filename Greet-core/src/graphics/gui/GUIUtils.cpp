#include "GUIUtils.h"

#include <graphics/fonts/FontManager.h>
#include <logging/Log.h>
#include <utils/LogUtils.h>
#include <utils/StringUtils.h>

namespace Greet
{
  bool GUIUtils::GetBooleanFromXML(const XMLObject& object, const std::string& key, bool defaultValue)
  {
    if(object.HasAttribute(key))
      return GetBoolean(object.GetAttribute(key));
    return defaultValue;
  }

  Color GUIUtils::GetColorFromXML(const XMLObject& object, const std::string& key, const Color& defaultValue)
  {
    if(object.HasAttribute(key))
      return GetColor(object.GetAttribute(key));
    return defaultValue;
  }

  GUISize GUIUtils::GetGUISizeFromXML(const XMLObject& object, const std::string& key, const GUISize& defaultValue)
  {
    if(object.HasAttribute(key))
      return GetGUISize(object.GetAttribute(key));
    return defaultValue;
  }

  std::string GUIUtils::GetStringFromXML(const XMLObject& object, const std::string& key, const std::string& defaultValue)
  {
    if(object.HasAttribute(key))
      return object.GetAttribute(key);
    return defaultValue;
  }

  int GUIUtils::GetIntFromXML(const XMLObject& object, const std::string& key, int defaultValue)
  {
    if(object.HasAttribute(key))
    {
      const std::string& str = object.GetAttribute(key);
      char* endPos;
      int value = std::strtol(str.c_str(), &endPos, 10);
      if(endPos != &*str.end())
        Log::Warning("Invalid string in int property \"%s\"=\"%s\"", key, str);
      return value;
    }
    return defaultValue;
  }

  float GUIUtils::GetFloatFromXML(const XMLObject& object, const std::string& key, float defaultValue)
  {
    if(object.HasAttribute(key))
      return GetFloat(object.GetAttribute(key));
    return defaultValue;
  }


  bool GUIUtils::GetBoolean(const std::string& str)
  {
    if (str == "true")
      return true;
    else if (str != "false")
      Log::Warning("Invalid string in boolean property \"%s\"", str);
    return false;
  }

  Color GUIUtils::GetColor(const std::string& str)
  {
    if (str[0] == '#')
    {
      std::string color = str.substr(1);
      if (color.length() != 6 && color.length() != 8)
      {
        Log::Error("Invalid length for color: ", str);
        return Color(1, 0, 1, 1); // Invalid color pink since its very visible
      }
      if (color.length() == 6)
        color = "FF" + color; // Add alpha to color
      return Color(LogUtils::HexToDec(color));
    }
    Log::Error("Invalid starting character for color: ", str);
    return Color(1, 0, 1, 1); // Invalid color pink since its very visible
  }

  float GUIUtils::GetFloat(const std::string& str)
  {
    char* endPos;
    float value = std::strtof(str.c_str(), &endPos);
    if(endPos != &*str.end())
      Log::Warning("Invalid string in float property: ", str);
    return value;
  }

  int GUIUtils::GetInt(const std::string& str)
  {
    char* endPos;
    int value = std::strtol(str.c_str(), &endPos, 10);
    if(endPos != &*str.end())
      Log::Warning("Invalid string in int property: ", str);
    return value;
  }

  GUISize GUIUtils::GetGUISize(const std::string& str)
  {
    GUISize sizing;
    char* endPos;
    sizing.value = std::strtof(str.c_str(), &endPos);
    if(str == "fill_parent")
    {
      sizing.type = GUISize::Type::Weight;
      sizing.value = 1;
    }
    else if(str == "wrap_content")
      sizing.type = GUISize::Type::Wrap;
    else if(*endPos == 'w')
      sizing.type  = GUISize::Type::Weight;
    else
    {
      if(endPos != &*str.end())
        Log::Warning("Invalid string in size property \"%s\"", str);
      sizing.type = GUISize::Type::Pixels;
    }
    return sizing;
  }

  Font GUIUtils::GetFont(const std::string& str)
  {
    size_t p = str.find(":");
    std::string fontName;
    int fontSize = 12;

    if(p == std::string::npos)
      fontName = str;
    else
    {
      fontName = str.substr(0, p);
      char* endPos;
      fontSize = std::strtol(str.c_str() + p + 1,  &endPos, 10);
      if(endPos != &*str.end())
        Log::Warning("Invalid fontSize in font: ", str.c_str() + p + 1);
    }
    if(fontSize <= 0)
      fontSize = 12;
    return FontManager::Get(fontName, fontSize);
  }

  TLBR GUIUtils::GetTLBR(const std::string& str)
  {
    TLBR tlbr;
    std::vector<std::string> strings = StringUtils::SplitString(str, " ");
    if(strings.size() == 4)
    {
      tlbr.top = atof(strings.at(0).c_str());
      tlbr.left = atof(strings.at(1).c_str());
      tlbr.bottom = atof(strings.at(2).c_str());
      tlbr.right = atof(strings.at(3).c_str());
    }
    else if(strings.size() == 2)
    {
      tlbr.top = tlbr.bottom = atof(strings.at(0).c_str());
      tlbr.left = tlbr.right = atof(strings.at(1).c_str());
    }
    else if(strings.size() == 1)
    {
      tlbr.top = tlbr.left = tlbr.bottom = tlbr.right = atof(strings.at(0).c_str());
    }
    else
    {
      Log::Error("TLBR:: Invalid parameter for ", str);
    }
    return tlbr;
  }
}
